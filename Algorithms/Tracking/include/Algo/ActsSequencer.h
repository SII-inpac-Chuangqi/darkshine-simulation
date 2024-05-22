//
// Created by xuliang on 2024/1/31.
//

#ifndef TRACKING_ACTSSEQUENCER_H
#define TRACKING_ACTSSEQUENCER_H

#include "Core/AnaProcessor.h"
#include "Core/AnaData.h"
#include "Algo/TrkHit.h"
#include "Algo/ActsSequencerContext.h"
#include "Algo/DPropagator.h"

#include <TParticlePDG.h>
#include <TDatabasePDG.h>

#include <cassert>

#define MeV_to_GeV 0.001



class ActsSequencer : public AnaProcessor {
public:
    explicit ActsSequencer(string name, shared_ptr<EventStoreAndWriter> evtwrt);
    ~ActsSequencer() {};

    void Begin() override;

    void InitEvt() override;

    void ProcessEvt(AnaEvent* evt) override;

    void CheckEvt(AnaEvent* evt) override;

    void End() override;

private:
    void ReadTrackerInfo();
    ActsFatras::ProcessType getProcessType(const TString& processName);
    ActsFatras::ProcessType getProcessType(int idx);
    void SimHitsReader(AnaEvent *evt, ActsExamples::AlgorithmContext context, tracking::detector detector);
    void ParticleReader(AnaEvent *evt, ActsExamples::AlgorithmContext context, tracking::detector detector);

    inline bool IsHitOnTracker(ActsFatras::Hit::Vector4 pos4, int CellId, tracking::detector detector) {
        if ( fabs(pos4.y()) > 0.5 * tracker_infos.at(detector).layer_lengths.at(CellId) )
            return false;
        return true;
    }

    static inline double GetHitY(const TrkHit& hit1, const TrkHit& hit2, double angle) {
        return hit2.GetY() + ((hit1.GetX() - hit2.GetX()) / tan(angle));
    }

    void InitializeSimHitsReader(tracking::detector detector);
    void InitializeSimParticlesReader(tracking::detector detector);
    void InitializeTrajectoryReader(tracking::detector detector);

private:
    // Tracker Info
    int if_strip{1};
    int use_dmagnet{0};
    double const_bfield{0};
    double ckf_selection_chi2max{0};
    int ckf_selection_nmax{0};
    int if_smear{1};

    struct TrackerInfo {
        void createArguments(bool use_dmagnet, double const_bfield, double ckf_selection_chi2max, int ckf_selection_nmax) {
            double max_widths = *std::max_element(layer_widths.begin(), layer_widths.end());
            double max_lengths = *std::max_element(layer_lengths.begin(), layer_lengths.end());
            double bound = max_widths > max_lengths ? max_widths : max_lengths;
            std::string tele_bound = std::to_string(std::ceil(bound)) + ":" + std::to_string(std::ceil(bound));
            std::string tele_positions;
            std::string tele_steros;
            for (int i = 0; i < (int)layer_z.size(); i++) {
                if (i % 2 == 1) continue;
                tele_positions += std::to_string(layer_z.at(i)) + ":";
                tele_steros += std::to_string(angles.at(i + 1)) + ":";
            }
            tele_positions.pop_back();
            tele_steros.pop_back();

            arguments.emplace_back("/");
            arguments.emplace_back(std::string("--geo-tele-bounds=") + tele_bound);
            arguments.emplace_back(std::string("--geo-tele-thickness=") + (layer_thickness.empty() ? "150" : std::to_string(100 * layer_thickness.front())));
            arguments.emplace_back(std::string("--geo-tele-positions=") + tele_positions);
            arguments.emplace_back(std::string("--geo-tele-stereos=") + tele_steros);
            arguments.emplace_back("--geo-tele-alignaxis=0");
            if (! use_dmagnet) {
                arguments.emplace_back(std::string("--bf-constant-tesla=0:0:") + std::to_string(-const_bfield));
            }
            arguments.emplace_back(std::string("--ckf-selection-chi2max=") + std::to_string(ckf_selection_chi2max));
            arguments.emplace_back(std::string("--ckf-selection-nmax=") + std::to_string(ckf_selection_nmax));
        }

        std::vector<int> strip_nums;
        std::vector<double> layer_widths;
        std::vector<double> layer_lengths;
        std::vector<double> layer_thickness;
        std::vector<double> layer_z;
        std::vector<double> angles;
        double region_front_z;
        double region_back_z;
        std::vector<std::string> arguments{};
    };

    std::map<tracking::detector, TrackerInfo> tracker_infos;

    // ====================================================
    // ACTS
    // ----------------------------------------------------
    using VertexCollection =
            std::vector<Acts::Vertex<Acts::BoundTrackParameters>>;

    int truth_smeared_seeded = true;
    std::map<tracking::detector, std::shared_ptr<ActsSequencerContext>> acts_contexts_;

    std::array<tracking::detector, 2> detectors_{tracking::dTag, tracking::dRec};
    std::map<tracking::detector, std::string> detector_names_{{tracking::dTag, "TagTrk"},
                                                              {tracking::dRec, "RecTrk"}};

    // Event Store Write Data Handles
    std::map<tracking::detector, ActsExamples::WriteDataHandle<ActsExamples::SimHitContainer>* > m_outputSimHits;
    std::map<tracking::detector, ActsExamples::WriteDataHandle<ActsExamples::SimParticleContainer>* > m_outputSimParticles;
    // Read Data Handles
    std::map<tracking::detector, ActsExamples::ReadDataHandle<ActsExamples::TrajectoriesContainer>* > m_inputTrajectories;
    std::map<tracking::detector, ActsExamples::ReadDataHandle<VertexCollection>* > m_inputVertices;

    // Magnetic field for propagation
    DPropagator propagator;
    TEveVectorD ECAL_Surface_Point{dAnaData->getECalSurfaceZ() / CUNIT, 0, 0}; // In Acts Frame
    TEveVectorD ECAL_Surface_Normal{1, 0, 0}; // In Acts Frame

    // ====================================================
    // Output Tracker Variables
    // ----------------------------------------------------
    struct TrackParams {
        // AtaPlane
        double d0{RETURN};
        double z0{RETURN};
        double phi{RETURN};
        double theta{RETURN};
        double P{RETURN};
        // trajectory summary info
        double chi2{RETURN};
    };
    struct TrackerOuptput {
        int No{0};
        std::vector<TrackParams> track{};
        // Flattened parameters
        std::vector<double> d0{};
        std::vector<double> z0{};
        std::vector<double> phi{};
        std::vector<double> theta{};
        std::vector<double> P{};
        std::vector<double> chi2{};
    };
    TrackerOuptput tag_trk_vars;
    TrackerOuptput rec_trk_vars;
    std::map<tracking::detector , TrackerOuptput*> acts_trk_vars{
        {tracking::detector::dTag, &tag_trk_vars},
        {tracking::detector::dRec, &rec_trk_vars}
    };
    // ====================================================
    // Output Vertex Variables
    // ----------------------------------------------------
    struct VertexParams {
        double x{RETURN};
        double y{RETURN};
        double z{RETURN};
        // track summary info
        double chi2{RETURN};
    };
    struct VertexOutput {
        int No{0};
        std::vector<VertexParams> vertex{};
        // Flattened parameters
        std::vector<double> x{};
        std::vector<double> y{};
        std::vector<double> z{};
        std::vector<double> chi2{};
    };
    VertexOutput tag_vtx_vars;
    VertexOutput rec_vtx_vars;
    std::map<tracking::detector , VertexOutput*> acts_vtx_vars{
        {tracking::detector::dTag, &tag_vtx_vars},
        {tracking::detector::dRec, &rec_vtx_vars}
    };
    // ====================================================
    // Output ECAL Seed Variables
    // ----------------------------------------------------
    struct ECalSeeds {
        std::vector<double> x;
        std::vector<double> y;
        std::vector<double> z;
        std::vector<double> px;
        std::vector<double> py;
        std::vector<double> pz;
    };
    ECalSeeds dECAL_seeds{};

    void AddECALSeed(const TrackParams &trk) {
        using namespace ActsHelper;
        // Initial position and momentum, Reference Frame: Acts, unit: GeV, cm, Tesla
        double charge = -1.0; // convention
        double pt = trk.P * MeV_to_GeV * sin(trk.theta);
        double pz = trk.P * MeV_to_GeV * cos(trk.theta);
        TEveVectorD vertex( -trk.d0 * sin(trk.phi) / CUNIT, trk.d0 * cos(trk.phi) / CUNIT, trk.z0 / CUNIT);
        TEveVectorD momentum(pt * cos(trk.phi), pt * sin(trk.phi), pz);
        TEveVectorD itsect_pos;
        TEveVectorD itsect_mom;

        propagator.InitTrack(vertex, charge);
        propagator.HelixIntersectPlane(momentum, ECAL_Surface_Point, ECAL_Surface_Normal, itsect_pos, itsect_mom);
        // from Acts Frame to Lab Frame
        itsect_pos = fromActsReferenceFrameD3(itsect_pos);
        itsect_mom = fromActsReferenceFrameD3(itsect_mom);
        dECAL_seeds.x.emplace_back(itsect_pos.fX * CUNIT);
        dECAL_seeds.y.emplace_back(itsect_pos.fY * CUNIT);
        dECAL_seeds.z.emplace_back(itsect_pos.fZ * CUNIT);
        dECAL_seeds.px.emplace_back(itsect_mom.fX / MeV_to_GeV);
        dECAL_seeds.py.emplace_back(itsect_mom.fY / MeV_to_GeV);
        dECAL_seeds.pz.emplace_back(itsect_mom.fZ / MeV_to_GeV);
    }

    PhysicsDef pDef;

};

#endif //TRACKING_ACTSSEQUENCER_H
