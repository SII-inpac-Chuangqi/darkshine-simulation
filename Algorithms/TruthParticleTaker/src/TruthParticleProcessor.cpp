//
// Created by Joseph Zhang on 10/31/23.
//
#include "Algorithms/TruthParticleTaker/include/Algo/TruthParticleProcessor.h"

//................................................................................//
//C++
#include <vector>
#include <memory>
#include <iostream>
#include <cmath>

//................................................................................//
//ROOT
#include <Math/Vector4D.h>
#include "TString.h"
#include <TParticlePDG.h>

//................................................................................//
//FRAMEWORK
#include "Core/AnaData.h"

//................................................................................//
//TRACKING
#include "Algo/TrkHit.h"
#include "Algo/Utils/Util.h"

TruthParticleProcessor::TruthParticleProcessor(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(
        std::move(name), std::move(evtwrt)){
    // Add description for this AnaProcessor
    Description = "Tracking truth hits and particles taken by Joseph ZHANG";
    RegisterIntParameter("verbose", "Verbose", &Verbose, 0);
    RegisterIntParameter("if_tagging", "If write truth info in tagging trackers", &if_tagging, 1);
    RegisterIntParameter("if_recoil", "If write truth info in recoil trackers", &if_recoil, 0);
}

TTree* TruthParticleProcessor::MakeTree() {
// Create a new TTree
    TTree *outputTree = new TTree("particles", "particles");
//................................................................................//
    outputTree->Branch("event_id", &event_id);
    outputTree->Branch("particle_id", &particle_id);
    outputTree->Branch("particle_type", &particle_type);   //PDG id
    outputTree->Branch("process", &process);
//initial vertexes space-time coordinates
    outputTree->Branch("vx", &vx);
    outputTree->Branch("vy", &vy);
    outputTree->Branch("vz", &vz);
    outputTree->Branch("vt", &vt);
//3-momentum and mass
    outputTree->Branch("px", &px);
    outputTree->Branch("py", &py);
    outputTree->Branch("pz", &pz);
    outputTree->Branch("m",  &m );
//tracking parameters
    outputTree->Branch("q", &q);
    outputTree->Branch("p", &p);
    outputTree->Branch("pt", &pt);
//others
    outputTree->Branch("eta", &eta);
    outputTree->Branch("phi", &phi);
    outputTree->Branch("vertex_primary", &vertex_primary);
    outputTree->Branch("vertex_secondary", &vertex_secondary);
    outputTree->Branch("particle",      &particle    );
    outputTree->Branch("generation",    &generation  );
    outputTree->Branch("sub_particle",  &sub_particle);

    return outputTree;
}

void TruthParticleProcessor::Begin() {
// Open a ROOT file for writing
//    TString outputFileName = "dp_particles.root"; // Figure out file name
//    outputFile = new TFile(outputFileName, "RECREATE");
    if (if_tagging) {
        fileT = new TFile(TString("dp_tag_particles.root"), "RECREATE");
        outputTreeT = MakeTree();
    }
    if (if_recoil) {
        fileR = new TFile(TString("dp_rcl_particles.root"), "RECREATE");
        outputTreeR = MakeTree();
    }
}

void TruthParticleProcessor::InitEvt() {
    std::vector<unsigned long>().swap(particle_id);
    std::vector<int>().swap(particle_type);   //PDG id
    vector<unsigned int>().swap(process);

    std::vector<float>().swap(vx);
    std::vector<float>().swap(vy);
    std::vector<float>().swap(vz);
    std::vector<float>().swap(vt);

    std::vector<float>().swap(px);
    std::vector<float>().swap(py);
    std::vector<float>().swap(pz);
    std::vector<float>().swap(m);

    std::vector<float>().swap(q);
    std::vector<float>().swap(p);
    std::vector<float>().swap(pt);
    std::vector<float>().swap(eta);
    std::vector<float>().swap(phi);

    vector<unsigned int>().swap(vertex_primary);
    vector<unsigned int>().swap(vertex_secondary);
    vector<unsigned int>().swap(particle);
    vector<unsigned int>().swap(generation);
    vector<unsigned int>().swap(sub_particle);

}

unsigned int TruthParticleProcessor::mapStringToUint(const std::string& str) const {
    static std::unordered_map<std::string, unsigned int> localStrToUintMap;
    // 如果映射表为空，初始化它
    if (localStrToUintMap.empty()) {
        // 建立字符串到 unsigned int 的映射
        for (size_t i = 1; i < physicsDef.dPhyTypeVec.size(); ++i) {
            localStrToUintMap[physicsDef.dPhyTypeVec[i]] = static_cast<unsigned int>(i);
        }
    }

    auto it = localStrToUintMap.find(str);
    if (it != localStrToUintMap.end()) {
        return it->second;
    } else {
        // 如果找不到，返回一个默认值，或者抛出异常，具体取决于你的需求
        return std::numeric_limits<unsigned int>::max();
    }
}

void TruthParticleProcessor::FillTruth(TTree *outputTree,
                                       DTruth *truth_info,
                                       std::vector<DStep *> *initial_steps,
                                       std::vector<McParticle *> *raw_mc_ptl )
{
    dAnaData->LoadTruthInfo(truth_info);

    //Figure out charged particles around TARGET in recoil region
    for (auto step : *initial_steps) {
        if (outputTree == outputTreeR && step->getPVName() == "World" && step->getZ() > -0.0 && step->getZ() < 0.5) //around the target
        {
            px.push_back(static_cast<float>(step->getPx()/1000.0f));
            py.push_back(static_cast<float>(step->getPy()/1000.0f));
            pz.push_back(static_cast<float>(step->getPz()/1000.0f));

            vx.push_back(step->getX());
            vy.push_back(step->getY());
            vz.push_back(step->getZ());
            vt.push_back(0.0f);  //DSS 没有记录打靶时间信息, 暂时搁置

            //truth momentum
            p.push_back( sqrt(
                    step->getPx() * step->getPx() + step->getPy() * step->getPy() + step->getPz() * step->getPz()
            ) / 1000.0f );

            //transverse momentum
            pt.push_back( sqrt(
                    step->getPx() * step->getPx() + step->getPy() * step->getPy()
            ) / 1000.0f);

            phi.push_back( std::atan( step->getPy() / step->getPx() ) );
            float theta = std::atan2( sqrt( step->getPx() * step->getPx() + step->getPy() * step->getPy() ), step->getPz());
            eta.push_back( -std::log(std::tan(theta / 2.0f)) );
        }
    }

    for (auto iptl : *raw_mc_ptl) {
        TParticlePDG* particlePDG = TDatabasePDG::Instance()->GetParticle(iptl->getPdg());
        if (particlePDG && particlePDG->Charge() != 0 ) {

            bool taggingCondition = outputTree == outputTreeT && iptl->getVertexZ() < -609.0 && iptl->getParents() == 0;
            bool recoilCondition = outputTree == outputTreeR && iptl->getVertexZ() > -0.175;

            //Tagging Selection
            if( taggingCondition) {
                px.push_back(0.0001f);
                py.push_back(0.0f);
                pz.push_back(static_cast<float>(iptl->getPz()/1000.0f));
                vx.push_back(iptl->getVertexX());
                vy.push_back(iptl->getVertexY());
                vz.push_back(iptl->getVertexZ());
                vt.push_back(0.0f);  //DSS 没有记录打靶时间信息, 暂时搁置
                //truth momentum
                p.push_back(static_cast<float>(iptl->getPz()/1000.0f));
                //transverse momentum
                pt.push_back(0.0001f);
                phi.push_back( 0.0f );
                eta.push_back( 6.0f );

                particle_id.push_back(iptl->getId());
                particle_type.push_back(iptl->getPdg()); // Get the electric charge of the particle
                q.push_back( particlePDG->Charge() / 3 );
                m.push_back(iptl->getMass()/1000.0f);
                auto processString = iptl->getCreateProcess();
                //std::string processString = "msc";
                if (!processString.empty()) {
                    process.push_back(mapStringToUint(processString));
                } else {
                    process.push_back(0); // 如果 processString 为空，可以根据需求添加默认值，这里添加0
                }
                vertex_primary.push_back(1);
                vertex_secondary.push_back(0);
                particle.push_back(iptl->getId());
                generation.push_back(0);
                sub_particle.push_back(0);
            }

            //Recoil Selection
            if( ( iptl->getParents() == 0 || recoilCondition ) && outputTree == outputTreeR){
                particle_id.push_back(iptl->getId());
                particle_type.push_back(iptl->getPdg());
                // Get the electric charge of the particle
                q.push_back( particlePDG->Charge() / 3 );
                m.push_back(iptl->getMass()/1000.0f);

                auto processString = iptl->getCreateProcess();
                //std::string processString = "msc";
                if (!processString.empty()) {
                    process.push_back(mapStringToUint(processString));
                } else {
                    // 如果 processString 为空，可以根据需求添加默认值，这里添加0
                    process.push_back(0);
                }
                vertex_primary.push_back(1);
                vertex_secondary.push_back(0);
                particle.push_back(iptl->getId());
                generation.push_back(0);
                sub_particle.push_back(0);

                //Particles emitted from the target can be classified into two types:
                //(1) Particles entering at -61 cm. We record their IDs since vertex information is already captured above.
                //(2) The secondary-particles resulting from nuclear reactions on the target.
                //    For charged particles in this category, we re-record their vertex information to avoid omissions or errors.

                if( recoilCondition && particlePDG->Charge() != 0 ) {
                    px.push_back(static_cast<float>(iptl->getPx()/1000.0f));
                    py.push_back(static_cast<float>(iptl->getPy()/1000.0f));
                    pz.push_back(static_cast<float>(iptl->getPz()/1000.0f));
                    vx.push_back(iptl->getVertexX());
                    vy.push_back(iptl->getVertexY());
                    vz.push_back(iptl->getVertexZ());
                    vt.push_back(0.0f);  //DSS 没有记录打靶时间信息, 暂时搁置
                    //truth momentum
                    p.push_back(sqrt(
                            iptl->getPx() * iptl->getPx() + iptl->getPy() * iptl->getPy() +
                            iptl->getPz() * iptl->getPz()
                    ) /1000.0f ) ;
                    //transverse momentum
                    pt.push_back(sqrt(
                            iptl->getPx() * iptl->getPx() + iptl->getPy() * iptl->getPy()
                    )/1000.0f);
                    phi.push_back( std::atan( iptl->getPy() / iptl->getPx() ) );
                    float theta = std::atan2( sqrt( iptl->getPx() * iptl->getPx() + iptl->getPy() * iptl->getPy() ), iptl->getPz());
                    eta.push_back( -std::log( std::tan(theta / 2.0f) ) );
                }
            }
        }
    }

    if (outputTree) { outputTree->Fill(); }
}

void TruthParticleProcessor::ProcessEvt(AnaEvent *evt) {

    const auto &step_collection = evt->getStepCollection();
    const auto &MCCollection = evt->getMcParticleCollection();
    [[maybe_unused]]const auto &simuhit_collection = evt->getSimulatedHitCollection();
    event_id = evt->getEventId();

    std::vector<DStep *> initial_steps;
    std::vector<McParticle *> raw_mc_ptl;

//    std::cout << "====================" << std::endl;

    if (if_tagging) {
        //Initialize vars
        this->InitEvt();
        initial_steps.clear();
        raw_mc_ptl.clear();
        for (auto istep : *step_collection.at("Initial_Particle_Step")) {
            initial_steps.emplace_back(istep);
        }
        for (auto iMcParticle : *MCCollection.at("RawMCParticle")) {
            raw_mc_ptl.emplace_back(iMcParticle);
        }
        this->FillTruth(outputTreeT, evt->getTruthInfo(), &initial_steps, &raw_mc_ptl);
    }

    if (if_recoil) {
        //Initialize vars
        this->InitEvt();
        initial_steps.clear();
        raw_mc_ptl.clear();
        for (auto istep : *step_collection.at("Initial_Particle_Step")) {
            initial_steps.emplace_back(istep);
        }
        for (auto iMcParticle : *MCCollection.at("RawMCParticle")) {
            raw_mc_ptl.emplace_back(iMcParticle);
        }
        this->FillTruth(outputTreeR, evt->getTruthInfo(), &initial_steps, &raw_mc_ptl);
    }

}

void TruthParticleProcessor::CheckEvt(AnaEvent *evt) {
    if (!evt) cerr << "[Warning] ==> Empty event" << endl;
}

void TruthParticleProcessor::End() {
    if (if_tagging && fileT) {
        fileT->Write();
        fileT->Close();
    }
    if (if_recoil && fileR) {
        fileR->Write();
        fileR->Close();
    }
    //cout<<"End!"<<endl;
}
