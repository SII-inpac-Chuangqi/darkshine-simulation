//
// Created by xuliang on 2024/1/31.
//

#if BUILD_ACTS

#include "Algo/ActsSequencer.h"
#include "Algo/ActsHelper.h"
#include <iostream>
#include <cassert>

ActsSequencer::ActsSequencer(std::string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(
        std::move(name), std::move(evtwrt)) {
    Description = "Acts Algorithm sequence";

    RegisterIntParameter("truth_smeared_seeded", "Use track parameters smeared from truth particles for steering CKF", &truth_smeared_seeded, 1);
    RegisterIntParameter("use_dmagnet", "1: Use Read non-constant magnetic filed from DMagnet. 0: Use constant magnetic field", &use_dmagnet, 1);
    RegisterDoubleParameter("const_bfiled", "Constant magnetic field By [Tesla] (Used with use_dmagnet=0)", &const_bfield, -1.5);
    RegisterDoubleParameter("ckf_selection_chi2max", "Maximum chi2 for CKF measurement selection", &ckf_selection_chi2max, 200);
    RegisterDoubleParameter("particle_selector_ptmin", "Minimum pT of truth particle for truth seeding (Recoil Tracker)", &particle_selector_ptmin, 0);
    RegisterIntParameter("ckf_selection_nmax", "Maximum number of measurement candidates on a surface for CKF measurement selection", &ckf_selection_nmax, 10);
}


void ActsSequencer::InitEvt() {
    tag_trk_vars = {};
    rec_trk_vars = {};
    tag_vtx_vars = {};
    rec_vtx_vars = {};
    dECAL_seeds = {};
}

void ActsSequencer::ReadTrackerInfo() {
    if(!if_strip) {
        std::cerr << "[WARNING] ==> Strip model not loaded in tracking" << std::endl;
        return;
    }

    if (!tracker_infos.empty())
        return;

    tracker_infos[tracking::dTag] = {
        dAnaData->getStripNoTag(),
        dAnaData->getLayerWidthTag(),
        dAnaData->getLayerLengthTag(),
        dAnaData->getLayerThicknessTag(),
        dAnaData->getLayerCentersZTag(),
        dAnaData->getAnglesTag(),
        dAnaData->getCenterZTag() - 0.5 * dAnaData->getLengthZTag(),
        dAnaData->getCenterZTag() + 0.5 * dAnaData->getLengthZTag(),
        {}
    };
    tracker_infos.at(tracking::dTag).createArguments(use_dmagnet, const_bfield, ckf_selection_chi2max, ckf_selection_nmax);

    tracker_infos[tracking::dRec] = {
        dAnaData->getStripNoRec(),
        dAnaData->getLayerWidthRec(),
        dAnaData->getLayerLengthRec(),
        dAnaData->getLayerThicknessRec(),
        dAnaData->getLayerCentersZRec(),
        dAnaData->getAnglesRec(),
        dAnaData->getCenterZRec() - 0.5 * dAnaData->getLengthZRec(),
        dAnaData->getCenterZRec() + 0.5 * dAnaData->getLengthZRec(),
        {}
    };
    tracker_infos.at(tracking::dRec).createArguments(use_dmagnet, const_bfield, ckf_selection_chi2max, ckf_selection_nmax);

    std::cout << "[INFO] ==> Strip model loaded in Acts" << std::endl;
}

void ActsSequencer::InitializeSimHitsReader(tracking::detector detector) {
    auto curActsContext = acts_contexts_.at(detector);
    m_outputSimHits.insert_or_assign(
            detector,
            new ActsExamples::WriteDataHandle<ActsExamples::SimHitContainer>(
                    new ActsExamples::CsvSimHitReader(
                            curActsContext->sim_hit_reader_cfg_,
                            curActsContext->logLevel
                    ),
                    curActsContext->sim_hit_reader_cfg_.outputSimHits
            )
    );
    m_outputSimHits.at(detector)->initialize(curActsContext->sim_hit_reader_cfg_.outputSimHits);
}

void ActsSequencer::SimHitsReader(AnaEvent *evt, ActsExamples::AlgorithmContext context, tracking::detector detector) {
    const auto &simuhit_collection = evt->getSimulatedHitCollection();
    auto trk_info = tracker_infos.at(detector);

    std::vector<TrkHit> raw_trk1_hits;
    std::vector<TrkHit> raw_trk2_hits;
    for (auto hit: *simuhit_collection.at(detector_names_.at(detector) + "1")) {
        raw_trk1_hits.emplace_back(*hit);
    }
    for (auto hit: *simuhit_collection.at(detector_names_.at(detector) + "2")) {
        raw_trk2_hits.emplace_back(*hit);
    }
    ActsExamples::SimHitContainer::sequence_type unordered;
    if (!if_strip)
        return; // TODO
    for (const auto &hit2: raw_trk2_hits) {
        // Always assume no rotation on tracker 1, and tracker 2 has rotation jk
        double angle = trk_info.angles.at(2 * hit2.GetCellIdZ() - 1);
        for (const auto &hit1: raw_trk1_hits) {
            if (!hit1.getPContribution().size()) continue;
            if (hit1.GetCellIdZ() != hit2.GetCellIdZ())
                continue;

            ActsFatras::Hit::Vector4 pos4{
                    hit1.GetX(),
                    GetHitY(hit1, hit2, angle),
                    hit1.GetZ() ,
                    hit1.getT(),
            };
            if ( !IsHitOnTracker(pos4, hit1.getCellId() - 1, detector) )
                continue;

            auto pcontrib = hit1.getPContribution().front();
            ActsFatras::Hit::Vector4 mom4{
                    pcontrib.getPx() * MeV_to_GeV,
                    pcontrib.getPy() * MeV_to_GeV,
                    pcontrib.getPz() * MeV_to_GeV,
                    pcontrib.getEnergy() * MeV_to_GeV
            };
            ActsFatras::Hit::Vector4 delta4{
                    0,
                    0,
                    -hit1.GetE() * MeV_to_GeV,
                    -hit1.GetE() * MeV_to_GeV
            };

            // FIXME: should read from geom!
            uint64_t geometry_id = 72057731476881664 +
                                   137438953472 * (hit1.getCellId() - 1);
            Acts::GeometryIdentifier geoId;
            geoId.setVolume(1);
            geoId.setSensitive(hit1.getCellId());
            // Transform to Acts Reference Frame
            ActsFatras::Hit hit(geometry_id,
                                pcontrib.getId(),
                                ActsHelper::toActsReferenceFrameV4(pos4),
                                ActsHelper::toActsReferenceFrameV4(mom4),
                                ActsHelper::toActsReferenceFrameV4(ActsFatras::Hit::Vector4(mom4 + delta4)),
                                hit1.getCellId() - 1);
            unordered.emplace_back(std::move(hit));
        }
    }

    // write the ordered data to the EventStore (according to geometry_id).
    ActsExamples::SimHitContainer simHits;
    simHits.insert(unordered.begin(), unordered.end());
    (*m_outputSimHits.at(detector))(context, std::move(simHits));
}

ActsFatras::ProcessType ActsSequencer::getProcessType(const TString &processName) {
    if (processName.Contains("Decay"))
        return ActsFatras::ProcessType::eDecay;
    if (processName.Contains("conv"))
        return ActsFatras::ProcessType::ePhotonConversion;
    if (processName.Contains("eBrem"))
        return ActsFatras::ProcessType::eBremsstrahlung;
    if (processName.Contains("Nuclear"))
        return ActsFatras::ProcessType::eNuclearInteraction;
    return ActsFatras::ProcessType::eUndefined;
}

ActsFatras::ProcessType ActsSequencer::getProcessType(int idx) {
    try {
        return getProcessType(pDef.dPhyTypeVec.at(idx));
    } catch (const std::exception &e) {
        cerr << "[Physics Process Table] ==> Out of range error for index: " << idx << endl;
        return ActsFatras::ProcessType::eUndefined;
    }
}

void ActsSequencer::InitializeSimParticlesReader(tracking::detector detector) {
    auto curActsContext = acts_contexts_.at(detector);
    m_outputSimParticles.insert_or_assign(
            detector,
            new ActsExamples::WriteDataHandle<ActsExamples::SimParticleContainer>(
                    new ActsExamples::CsvParticleReader(
                            curActsContext->particle_reader_cfg_,
                            curActsContext->logLevel
                    ),
                    curActsContext->particle_reader_cfg_.outputParticles
            )
    );
    m_outputSimParticles.at(detector)->initialize(curActsContext->particle_reader_cfg_.outputParticles);
}

void ActsSequencer::InitializeTrajectoryReader(tracking::detector detector) {
    auto curActsContext = acts_contexts_.at(detector);
    m_inputTrajectories.insert_or_assign(
            detector,
            new ActsExamples::ReadDataHandle<ActsExamples::TrajectoriesContainer>(
                    curActsContext->sequenceElements.back().get(),
                    curActsContext->tracks_to_traj_cfg_.outputTrajectories
            )
    );
    m_inputTrajectories.at(detector)->initialize(curActsContext->tracks_to_traj_cfg_.outputTrajectories);

    m_inputVertices.insert_or_assign(
            detector,
            new ActsExamples::ReadDataHandle<VertexCollection>(
                curActsContext->sequenceElements.back().get(),
                curActsContext->vertex_finder_cfg_.outputVertices
            )
    );
    m_inputVertices.at(detector)->initialize(curActsContext->vertex_finder_cfg_.outputVertices);
}

void ActsSequencer::ParticleReader(AnaEvent *evt, ActsExamples::AlgorithmContext context, tracking::detector detector) {
    ActsExamples::SimParticleContainer::sequence_type unordered;
    auto trk_info = tracker_infos.at(detector);
    const auto &truth_tracks = evt->getTruthInfo()->getTracksInRegion(detector == tracking::dTag ? DTruth::DTruthDetPV::TagTrk : DTruth::DTruthDetPV::RecTrk);
    for (const auto &track: truth_tracks) {
        int id = track.first.first;
        int pdg = track.first.second;
        auto state = track.second.front();
        auto pdgData =  TDatabasePDG::Instance()->GetParticle(pdg);
        if (!pdgData) continue;
        double charge = pdgData->Charge() / 3.0;
        double mass = pdgData->Mass();
        ActsFatras::Particle particle(ActsFatras::Barcode(id),
                                      Acts::PdgParticle(pdg),
                                      charge * Acts::UnitConstants::e,
                                      mass * Acts::UnitConstants::GeV);
        particle.setProcess(getProcessType(state->process_index));
        ActsFatras::Particle::Vector4 vertex {
                state->vertex[0] * Acts::UnitConstants::mm,
                state->vertex[1] * Acts::UnitConstants::mm,
                state->vertex[2] * Acts::UnitConstants::mm,
                0 * Acts::UnitConstants::ns
        };
        ActsFatras::Particle::Vector3 momentum {
                state->momentum[0] * MeV_to_GeV,
                state->momentum[1] * MeV_to_GeV,
                state->momentum[2] * MeV_to_GeV
        };
        particle.setPosition4(ActsHelper::toActsReferenceFrameV4(vertex));
        // Only used for direction; normalization/units do not matter
        ActsFatras::Particle::Vector3 momentum_new = ActsHelper::toActsReferenceFrameV3(momentum);
        particle.setDirection(momentum_new);
        particle.setAbsoluteMomentum(std::hypot(momentum_new.x(), momentum_new.y(), momentum_new.z()) *
                                     Acts::UnitConstants::GeV);
        unordered.emplace_back(std::move(particle));
    }
    // Get new mc particles produced inside region
    const auto &mc_collection = evt->getMcParticleCollection();
    const auto &mc_particles = mc_collection.at("RawMCParticle");
    for (const auto &mcp : *mc_particles) {
        if (mcp->getVertexZ() <= trk_info.region_front_z) continue;
        if (mcp->getVertexZ() >= trk_info.region_back_z) continue;
        auto pdgData = TDatabasePDG::Instance()->GetParticle(mcp->getPdg());
        if (! pdgData) continue;
        double charge = pdgData->Charge() / 3.0;
        ActsFatras::Particle particle(ActsFatras::Barcode(mcp->getId()),
                                      Acts::PdgParticle(mcp->getPdg()),
                                      charge * Acts::UnitConstants::e,
                                      mcp->getMass() * Acts::UnitConstants::GeV);
        particle.setProcess(getProcessType(mcp->getCreateProcess()));
        ActsFatras::Particle::Vector4 vertex {
            mcp->getVertexX() * Acts::UnitConstants::mm,
            mcp->getVertexY() * Acts::UnitConstants::mm,
            mcp->getVertexZ() * Acts::UnitConstants::mm,
            0 * Acts::UnitConstants::ns
        };
        ActsFatras::Particle::Vector3 momentum {
                mcp->getPx() * MeV_to_GeV,
                mcp->getPy() * MeV_to_GeV,
                mcp->getPz() * MeV_to_GeV
        };
        particle.setPosition4(ActsHelper::toActsReferenceFrameV4(vertex));
        // Only used for direction; normalization/units do not matter
        ActsFatras::Particle::Vector3 momentum_new = ActsHelper::toActsReferenceFrameV3(momentum);
        particle.setDirection(momentum_new);
        particle.setAbsoluteMomentum(std::hypot(momentum_new.x(), momentum_new.y(), momentum_new.z()) *
                                     Acts::UnitConstants::GeV);
        unordered.emplace_back(std::move(particle));
    }

    // Write ordered particles container to the EventStore
    ActsExamples::SimParticleContainer particles;
    particles.insert(unordered.begin(), unordered.end());
    (*m_outputSimParticles.at(detector))(context, std::move(particles));

}

void ActsSequencer::Begin() {
    EvtWrt->RegisterOutVariable("Acts_TagTrk_No", &(tag_trk_vars.No), "Reconstructed number of track in tagging tracker");
    EvtWrt->RegisterOutVariable("Acts_RecTrk_No", &(rec_trk_vars.No), "Reconstructed number of track in recoil tracker");
    EvtWrt->RegisterOutVariable("Acts_TagTrk_P", &(tag_trk_vars.P), "Reconstructed track momentum in tagging tracker");
    EvtWrt->RegisterOutVariable("Acts_RecTrk_P", &(rec_trk_vars.P), "Reconstructed track momentum in recoil tracker");
    EvtWrt->RegisterOutVariable("Acts_TagTrk_chi2", &(tag_trk_vars.chi2), "Reconstructed track chi^2 in tagging tracker");
    EvtWrt->RegisterOutVariable("Acts_RecTrk_chi2", &(rec_trk_vars.chi2), "Reconstructed track chi^2 in recoil tracker");
    EvtWrt->RegisterOutVariable("Acts_RecTrk_chi2", &(rec_trk_vars.chi2), "Reconstructed track chi^2 in recoil tracker");
    EvtWrt->RegisterOutVariable("Acts_ECal_seed_x",  &dECAL_seeds.x);
    EvtWrt->RegisterOutVariable("Acts_ECal_seed_y",  &dECAL_seeds.y);
    EvtWrt->RegisterOutVariable("Acts_ECal_seed_z",  &dECAL_seeds.z);
    EvtWrt->RegisterOutVariable("Acts_ECal_seed_px", &dECAL_seeds.px);
    EvtWrt->RegisterOutVariable("Acts_ECal_seed_py", &dECAL_seeds.py);
    EvtWrt->RegisterOutVariable("Acts_ECal_seed_pz", &dECAL_seeds.pz);

    EvtWrt->RegisterOutVariable("Acts_RecVtx_No", &(rec_vtx_vars.No), "Reconstructed number of track in recoil tracker");
    EvtWrt->RegisterOutVariable("Acts_RecVtx_x", &(rec_vtx_vars.x), "Reconstructed track momentum in recoil tracker");
    EvtWrt->RegisterOutVariable("Acts_RecVtx_y", &(rec_vtx_vars.y), "Reconstructed track momentum in recoil tracker");
    EvtWrt->RegisterOutVariable("Acts_RecVtx_z", &(rec_vtx_vars.z), "Reconstructed track momentum in recoil tracker");
    EvtWrt->RegisterOutVariable("Acts_RecVtx_chi2", &(rec_vtx_vars.chi2), "Reconstructed track chi^2 in recoil tracker");

    ReadTrackerInfo();

    // TODO: non-constant magnetic field
    // TODO: Material config
    acts_contexts_[tracking::dTag] = std::make_shared<ActsSequencerContext>();
    acts_contexts_.at(tracking::dTag)->detector_type = tracking::dTag;
    acts_contexts_.at(tracking::dTag)->truthSmearedSeeded = truth_smeared_seeded;
    acts_contexts_.at(tracking::dTag)->useDMagnet = use_dmagnet;
    acts_contexts_.at(tracking::dTag)->particle_selector_ptmin = 100._MeV;
    acts_contexts_.at(tracking::dTag)->setConstantBField(const_bfield);
    acts_contexts_.at(tracking::dTag)->setup(tracker_infos.at(tracking::dTag).arguments);
    acts_contexts_[tracking::dRec] = std::make_shared<ActsSequencerContext>();
    acts_contexts_.at(tracking::dRec)->detector_type = tracking::dRec;
    acts_contexts_.at(tracking::dRec)->truthSmearedSeeded = truth_smeared_seeded;
    acts_contexts_.at(tracking::dRec)->useDMagnet = use_dmagnet;
    acts_contexts_.at(tracking::dRec)->particle_selector_ptmin = particle_selector_ptmin  * Acts::UnitConstants::MeV;
    acts_contexts_.at(tracking::dRec)->setConstantBField(const_bfield);
    acts_contexts_.at(tracking::dRec)->setup(tracker_infos.at(tracking::dRec).arguments);
    assert(m_outputSimHits.empty());
    assert(m_outputSimParticles.empty());
    assert(m_inputTrajectories.empty());
    assert(m_inputVertices.empty());
    for (auto detector : detectors_) {
        auto curActsContext = acts_contexts_.at(detector);
        // initialize Readers
        InitializeSimHitsReader(detector);
        if (curActsContext->truthSmearedSeeded) {
            InitializeSimParticlesReader(detector);
        }
        // Initialize Algorithms
        for (const auto& alg : curActsContext->sequenceElements) {
            alg->initialize();
        }
        // retrieve the trajectory
        InitializeTrajectoryReader(detector);
    }
    // Initialize propagator
    propagator.SetMagFieldObj(new DActsTEveMagField());
    propagator.SetMaxR(50);
    propagator.SetMaxZ(100);
    propagator.SetMaxStep(2);
}

void ActsSequencer::ProcessEvt(AnaEvent *evt) {
    InitEvt();
    // Run Acts algorithms
    for (auto detector : detectors_) {
        // Event Store. store arbitary data with ownership transfer
        ActsExamples::WhiteBoard eventStore(Acts::getDefaultLogger("EventStore#" + std::to_string(evt->getEventId()),
                                                                   acts_contexts_.at(detector)->logLevel),
                                            acts_contexts_.at(detector)->whiteboardObjectAliases);
        // Aggregated information to run one algorithm over one event
        ActsExamples::AlgorithmContext context(0, evt->getEventId(), eventStore);
        // Transfer data from DEvent to Acts Event Store
        SimHitsReader(evt, ++context, detector);
        if (acts_contexts_.at(detector)->truthSmearedSeeded) {
            ParticleReader(evt, ++context, detector);
        }
        // Execute Acts algorithm sequence
        for (const auto& alg : acts_contexts_.at(detector)->sequenceElements) {
            alg->internalExecute(++context);
        }
        // ====================================================
        // Output Tracker Variables
        // ----------------------------------------------------
        // loop over the trajectories
        auto inputHandle = m_inputTrajectories.at(detector);
        auto cur_trk_vars = acts_trk_vars.at(detector);
        auto trajectories = (*inputHandle)(++context);
        cur_trk_vars->No = (int)trajectories.size();
        for (const auto &traj : trajectories) {
            const auto& mj = traj.multiTrajectory();
            // fill track parameters
            if (traj.tips().empty()) {
                cur_trk_vars->No -= 1; // not track
                continue;
            }
            const auto &trackTip = traj.tips().front();
            const auto params = traj.trackParameters(trackTip).parameters();
            auto trajState = Acts::MultiTrajectoryHelpers::trajectoryState(mj, trackTip);
            cur_trk_vars->track.emplace_back(
                TrackParams({
                    params[Acts::eBoundLoc0],
                    params[Acts::eBoundLoc1],
                    params[Acts::eBoundPhi],
                    params[Acts::eBoundTheta],
                    params[Acts::eBoundQOverP] != 0 ? -1.0 / params[Acts::eBoundQOverP] / MeV_to_GeV : RETURN, // assume Q = -1
                    trajState.chi2Sum
                })
            );
        }
        // sort by momentum
        std::sort(cur_trk_vars->track.begin(), cur_trk_vars->track.end(),
            [](const auto& l, const auto& r) {
                return std::abs(l.P) > std::abs(r.P);
        });
        // fill flattened parameters
        for (const auto& track : cur_trk_vars->track) {
            cur_trk_vars->d0.emplace_back(track.d0);
            cur_trk_vars->z0.emplace_back(track.z0);
            cur_trk_vars->phi.emplace_back(track.phi);
            cur_trk_vars->theta.emplace_back(track.theta);
            cur_trk_vars->P.emplace_back(track.P);
            cur_trk_vars->chi2.emplace_back(track.chi2);
        }
        // ====================================================
        // Output Vertex Variables
        // ----------------------------------------------------
        auto input_vertices_handle = m_inputVertices.at(detector);
        auto cur_vtx_vars = acts_vtx_vars.at(detector);
        auto vertex_collection = (*input_vertices_handle)(++context);
        cur_vtx_vars->No = (int)vertex_collection.size();
        for (const auto &vtx : vertex_collection) {
            auto vtx_dss = ActsHelper::fromActsReferenceFrameV3(vtx.position());
            cur_vtx_vars->vertex.emplace_back(
                VertexParams({
                    vtx_dss.x(),
                    vtx_dss.y(),
                    vtx_dss.z(),
                    vtx.fitQuality().first
                })
            );
        }
        // sort by z position (DSS Frame)
        std::sort(cur_vtx_vars->vertex.begin(), cur_vtx_vars->vertex.end(),
             [](const auto& l, const auto& r) {
                 return l.z < r.z;
             }
        );
        // fill flattened parameters
        for (const auto& vtx : cur_vtx_vars->vertex) {
            cur_vtx_vars->x.emplace_back(vtx.x);
            cur_vtx_vars->y.emplace_back(vtx.y);
            cur_vtx_vars->z.emplace_back(vtx.z);
            cur_vtx_vars->chi2.emplace_back(vtx.chi2);
        }
        // ====================================================
        // Output ECAL Seed Variables
        // ----------------------------------------------------
        // Propagate recoil track and Fill ECAL Seed
        if (detector == tracking::dRec) {
            for (const auto &track: cur_trk_vars->track) {
                AddECALSeed(track);
            }
        }

    }
}

void ActsSequencer::CheckEvt(AnaEvent *evt) {
    if (!evt) cerr << "[Warning] ==> Empty event" << endl;
}

void ActsSequencer::End() {
    for (auto detector : detectors_) {
        for (const auto& alg : acts_contexts_.at(detector)->sequenceElements) {
            alg->finalize();
        }
    }
}

#endif
