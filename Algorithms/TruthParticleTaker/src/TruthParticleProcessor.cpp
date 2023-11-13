//
// Created by Joseph Zhang on 10/31/23.
//
#include "Algorithms/TruthParticleTaker/include/Algo/TruthParticleProcessor.h"

//................................................................................//
//C++
#include <vector>
#include <memory>
#include <iostream>

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
}

void TruthParticleProcessor::Begin() {
// Open a ROOT file for writing
    TString outputFileName = "dp_particles.root"; // Figure out file name
    outputFile = new TFile(outputFileName, "RECREATE");
// Create a new TTree
    outputTree = new TTree("particles", "particles");
//................................................................................//
//Truth
//................................................................................//
    outputTree->Branch("event_id", &event_id, "event_id/I");
    outputTree->Branch("particle_id", &particle_id);
    outputTree->Branch("particle_type", &particle_type);   //PDG id
    //outputTree->Branch("process", &process);
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
}

void TruthParticleProcessor::InitEvt() {
    int event_id;
    std::vector<unsigned long>().swap(particle_id);
    std::vector<int>().swap(particle_type);   //PDG id
    //vector<unsigned int>().swap(process);

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
}

void TruthParticleProcessor::FillTruth(DTruth *truth_info, std::vector<DStep *> *initial_steps, std::vector<McParticle *> *raw_mc_ptl )
{
    dAnaData->LoadTruthInfo(truth_info);
//    bool trkTargetFlag = false;
    for (auto step : *initial_steps) {
        if (step->getPVName() == "World" && step->getZ() > -0.5 && step->getZ() < 0.5) //around the target
        {
            px.push_back(step->getPx());
            py.push_back(step->getPy());
            pz.push_back(step->getPz());

            vx.push_back(step->getX());
            vy.push_back(step->getY());
            vz.push_back(step->getZ());
            vt.push_back(0.0);  //DSS 没有记录打靶时间信息, 暂时搁置

            //truth momentum
            p.push_back( sqrt(
                    step->getPx() * step->getPx() + step->getPy() * step->getPy() + step->getPz() * step->getPz()
            ) );

            //transverse momentum
            pt.push_back( sqrt(
                    step->getPx() * step->getPx() + step->getPy() * step->getPy()
            ) );
            //trkTargetFlag = true;
        }
    }

    for (auto iptl : *raw_mc_ptl) {
        TParticlePDG* particlePDG = TDatabasePDG::Instance()->GetParticle(iptl->getPdg());
        if (particlePDG) {
            if( ( iptl->getParents() == 0 && particlePDG->Charge() != 0 ) || ( iptl->getVertexZ() > -0.18 && iptl->getVertexZ() < 0.18 && particlePDG->Charge() != 0 ) ) {
                particle_id.push_back(iptl->getId());
                particle_type.push_back(iptl->getPdg());
                // Get the electric charge of the particle
                q.push_back( particlePDG->Charge() / 3 );
                m.push_back(iptl->getMass());
                //从靶子开始发射的粒子分两类:
                // 第一类是-61入射的粒子, 我们记录它的id即可, 因为顶点信息已经写在上面了
                // 第二类是靶子上面核反应之后产生的新粒子, 如果带电, 我们就重新记录他们的顶点, 以免错漏
                if( iptl->getVertexZ() > -0.18 && iptl->getVertexZ() < 0.18 && particlePDG->Charge() != 0 ) {
                    px.push_back(iptl->getPx());
                    py.push_back(iptl->getPy());
                    pz.push_back(iptl->getPz());
                    vx.push_back(iptl->getVertexX());
                    vy.push_back(iptl->getVertexY());
                    vz.push_back(iptl->getVertexZ());
                    vt.push_back(0.0);  //DSS 没有记录打靶时间信息, 暂时搁置
                    //truth momentum
                    p.push_back(sqrt(
                            iptl->getPx() * iptl->getPx() + iptl->getPy() * iptl->getPy() +
                            iptl->getPz() * iptl->getPz()
                    ));
                    //transverse momentum
                    pt.push_back(sqrt(
                            iptl->getPx() * iptl->getPx() + iptl->getPy() * iptl->getPy()
                    ));
                }
            }
        }
    }

    outputTree->Fill();
}

void TruthParticleProcessor::ProcessEvt(AnaEvent *evt) {
//Initialize vars
    this->InitEvt();
    const auto &step_collection = evt->getStepCollection();
    const auto &MCCollection = evt->getMcParticleCollection();
    [[maybe_unused]]const auto &simuhit_collection = evt->getSimulatedHitCollection();

    event_id = evt->getEventId();
    const auto &initial_steps = step_collection.at("Initial_Particle_Step");
    const auto &raw_mc_ptl = MCCollection.at("RawMCParticle");

//    std::cout << "====================" << std::endl;

//Write truth
    this->FillTruth(evt->getTruthInfo(), initial_steps, raw_mc_ptl);
}

void TruthParticleProcessor::CheckEvt(AnaEvent *evt) {
    if (!evt) cerr << "[Warning] ==> Empty event" << endl;
}

void TruthParticleProcessor::End() {
    outputFile->Write();
    outputFile->Close();
    //cout<<"End!"<<endl;
}






