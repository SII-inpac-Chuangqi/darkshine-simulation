//
// Created by Zhang Yulei on 9/19/20.
//

#include "Object/DEvent.h"

#include "TObjectTable.h"
#include "TROOT.h"

#include "TStreamerInfo.h"

void DEvent::Initialization(CleanType ct) {
    // clean constant variables
    PNEnergy_Target = 0.;
    PNEnergy_ECAL = 0.;
    Eleak_ECAL = 0.;
    TotalRecEnergy = 0.;
    ECALRecEnergy = 0.;
    HCALRecEnergy = 0.;

    for (auto itr : MCParticleCollection) {
        for (auto itr2 : *itr.second) {
            delete itr2;
        }
        (itr.second)->clear();
        (itr.second)->shrink_to_fit();
        if (ct == nALL) delete itr.second;
    }
    if (ct == nALL) MCParticleCollection.clear();
    for (auto itr : RecParticleCollection) {
        for (auto itr2 : *itr.second) {
            delete itr2;
        }
        (itr.second)->clear();
        (itr.second)->shrink_to_fit();
        if (ct == nALL) delete itr.second;
    }
    if (ct == nALL) RecParticleCollection.clear();

    for (auto itr : SimulatedHitCollection) {
        for (auto itr2 : *itr.second) {
            delete itr2;
        }
        (itr.second)->clear();
        (itr.second)->shrink_to_fit();
        if (ct == nALL) delete itr.second;
    }
    if (ct == nALL) SimulatedHitCollection.clear();

    for (auto itr : CalorimeterHitCollection) {
        for (auto itr2 : *itr.second) {
            delete itr2;
        }
        (itr.second)->clear();
        (itr.second)->shrink_to_fit();
        if (ct == nALL) delete itr.second;
    }
    if (ct == nALL) CalorimeterHitCollection.clear();

    for (auto itr : StepCollection) {
        for (auto itr2 : *itr.second) {
            delete itr2;
        }
        (itr.second)->clear();
        (itr.second)->shrink_to_fit();
        if (ct == nALL) delete itr.second;
    }
    if (ct == nALL) StepCollection.clear();

    for (auto itr : OpticalCollection) {
        for (auto itr2 : *itr.second) {
            delete itr2;
        }
        (itr.second)->clear();
        (itr.second)->shrink_to_fit();
        if (ct == nALL) delete itr.second;
    }
    if (ct == nALL) OpticalCollection.clear();

}

void DEvent::PrintDetails()
{
    for(auto steps : StepCollection)
    {
        std::cout << "**************************************************************************************************************************"
                  << std::endl
                  << "* Step Collection: " << steps.first << std::endl
                  << "**************************************************************************************************************************"
                  << std::endl
                  << "| ID    |    x[mm]     y[mm]    z[mm]  |  Px[MeV]   Py[MeV]   Pz[MeV]    E[MeV] | PVName                 Process Name    |"
                  << std::endl
                  << "**************************************************************************************************************************"
                  << std::endl;
        for(auto step : *(steps.second))
            std::cout << *step << std::endl;
    }

    for(auto particles : MCParticleCollection)
    {
        std::cout << "****************************************************************************************************************************************************************************"
                  << std::endl
                  << "* MC Particle Collection: " << particles.first << std::endl
                  << "****************************************************************************************************************************************************************************"
                  << std::endl
                  << "| ID    | PDG   |  Px[MeV]   Py[MeV]   Pz[MeV] |   E[MeV]  Mass[MeV]  ER[MeV] |Vertex x[mm]     y[mm]     z[mm] |End x[mm]     y[mm]     z[mm] | Process          Parent   |"
                  << std::endl
                  << "****************************************************************************************************************************************************************************"
                  << std::endl;
         for(auto particle : *(particles.second))
             std::cout << *particle << std::endl;
    }

    for(auto simus : SimulatedHitCollection)
    {
        std::cout << "*********************************************************************" << std::endl
                  << "* Simulated Hit Collection: " << simus.first << std::endl
                  << "*********************************************************************" << std::endl
                  << "| ID    |    x[mm]     y[mm]     z[mm]  time[ns]    E[GeV] | Cell ID|" << std::endl
                  << "*********************************************************************" << std::endl;
        for(auto simu : *(simus.second))
             std::cout << *simu << std::endl;
    }
    std::cout << "*********************************************************************" << std::endl;
}

DStepVec *DEvent::RegisterStepCollection(const std::string &str) {
    if (StepCollection.count(str) != 0) {
        std::cerr << "[WARNING] ==> Key already exists. Return the existing Key: " << str << std::endl;
        return nullptr;
    }
    auto tmpVec = new DStepVec();
    StepCollection.emplace(std::pair<std::string, DStepVec *>(str, tmpVec));

    if (Verbose > 1) {
        std::cout << "[STEP REGISTER] : (Verbosity 2) ==> A new collection " + str +
                     " has been successfully added to MCParticle Collection." << std::endl;
    }

    return tmpVec;
}


DigiFormVec *DEvent::RegisterOpticalCollection(const std::string &str) {
    if (OpticalCollection.count(str) != 0) {
        std::cerr << "[WARNING] ==> Key already exists. Return the existing Key: " << str << std::endl;
        return nullptr;
    }
    auto tmpVec = new DigiFormVec();
    tmpVec->clear();
    OpticalCollection.emplace(std::pair<std::string, DigiFormVec *>(str, tmpVec));

    if (Verbose > 1) {
        std::cout << "[Optical REGISTER] : (Verbosity 2) ==> A new collection " + str +
                     " has been successfully added to MCParticle Collection." << std::endl;
    }

    return tmpVec;
}

MCParticleVec *DEvent::RegisterMCParticleCollection(const std::string &str) {
    if (MCParticleCollection.count(str) != 0) {
        std::cerr << "[WARNING] ==> Key already exists. Return the existing Key: " << str << std::endl;
        return nullptr;
    }
    auto tmpVec = new MCParticleVec();
    MCParticleCollection.emplace(std::pair<std::string, MCParticleVec *>(str, tmpVec));

    if (Verbose > 1) {
        std::cout << "[MC REGISTER] : (Verbosity 2) ==> A new collection " + str +
                     " has been successfully added to MCParticle Collection." << std::endl;
    }

    return tmpVec;
}


RecParticleVec *DEvent::RegisterRecParticleCollection(const std::string &str) {
    if (RecParticleCollection.count(str) != 0) {
        std::cerr << "[WARNING] ==> Key already exists. Return the existing Key: " << str << std::endl;
        return nullptr;
    }
    auto tmpVec = new RecParticleVec();
    RecParticleCollection.emplace(std::pair<std::string, RecParticleVec *>(str, tmpVec));

    if (Verbose > 1) {
        std::cout << "[REC REGISTER] : (Verbosity 2) ==> A new collection " + str +
                     " has been successfully added to RecParticle Collection." << std::endl;
    }

    return tmpVec;
}

SimulatedHitVec *DEvent::RegisterSimulatedHitCollection(const std::string &str) {
    if (SimulatedHitCollection.count(str) != 0) {
        std::cerr << "[WARNING] ==> Key already exists. Return the existing Key: " << str << std::endl;
        return nullptr;
    }
    auto tmpVec = new SimulatedHitVec();
    SimulatedHitCollection.emplace(std::pair<std::string, SimulatedHitVec *>(str, tmpVec));

    if (Verbose > 1) {
        std::cout << "[MC REGISTER] : (Verbosity 2) ==> A new collection " + str +
                     " has been successfully added to SimulatedHit Collection." << std::endl;
    }

    return tmpVec;
}

CalorimeterHitVec *DEvent::RegisterCalorimeterHitCollection(const std::string &str) {
    if (CalorimeterHitCollection.count(str) != 0) {
        std::cerr << "[WARNING] ==> Key already exists. Return the existing Key: " << str << std::endl;
        return nullptr;
    }
    auto tmpVec = new CalorimeterHitVec();
    CalorimeterHitCollection.emplace(std::pair<std::string, CalorimeterHitVec *>(str, tmpVec));

    if (Verbose > 1) {
        std::cout << "[REC REGISTER] : (Verbosity 2) ==> A new collection " + str +
                     " has been successfully added to CalorimterHit Collection" << std::endl;
    }

    return tmpVec;
}

std::vector<std::string> *DEvent::ListAllCollections() {
    auto tmp = new std::vector<std::string>;

    auto s = ListCollections(StepCollection);
    tmp->insert(tmp->end(), s->begin(), s->end());
    delete s;

    s = ListCollections(OpticalCollection);
    tmp->insert(tmp->end(), s->begin(), s->end());
    delete s;

    s = ListCollections(MCParticleCollection);
    tmp->insert(tmp->end(), s->begin(), s->end());
    delete s;

    s = ListCollections(RecParticleCollection);
    tmp->insert(tmp->end(), s->begin(), s->end());
    delete s;

    s = ListCollections(SimulatedHitCollection);
    tmp->insert(tmp->end(), s->begin(), s->end());
    delete s;

    s = ListCollections(CalorimeterHitCollection);
    tmp->insert(tmp->end(), s->begin(), s->end());
    delete s;

    return tmp;
}

void DEvent::DeleteCollection(const std::string &str) {
    auto itr1 = MCParticleCollection.find(str);
    auto itr2 = RecParticleCollection.find(str);
    auto itr3 = SimulatedHitCollection.find(str);
    auto itr4 = CalorimeterHitCollection.find(str);
    auto itr5 = StepCollection.find(str);
    auto itr6 = OpticalCollection.find(str);

    if (itr1 != MCParticleCollection.end()) {
        MCParticleCollection.erase(itr1);
        std::cout << "[MC DELETE] ==> Collection " + str + " has been successfully removed." << std::endl;
    } else if (itr2 != RecParticleCollection.end()) {
        RecParticleCollection.erase(itr2);
        std::cout << "[REC DELETE] ==> Collection " + str + " has been successfully removed." << std::endl;
    } else if (itr3 != SimulatedHitCollection.end()) {
        SimulatedHitCollection.erase(itr3);
        std::cout << "[SIM DELETE] ==> Collection " + str + " has been successfully removed." << std::endl;
    } else if (itr4 != CalorimeterHitCollection.end()) {
        CalorimeterHitCollection.erase(itr4);
        std::cout << "[CAL DELETE] ==> Collection " + str + " has been successfully removed." << std::endl;
    } else if (itr5 != StepCollection.end()) {
        StepCollection.erase(itr5);
        std::cout << "[STEP DELETE] ==> Collection " + str + " has been successfully removed." << std::endl;
    } else if (itr6 != OpticalCollection.end()) {
        OpticalCollection.erase(itr6);
        std::cout << "[Opt DELETE] ==> Collection " + str + " has been successfully removed." << std::endl;
    } else
        std::cerr << "[WARNING] ==> No Key named " + str + "." << std::endl;
}

void DEvent::LinkChildren() {
    if (MCParticleCollection.empty()) return;
    for (const auto &collection : MCParticleCollection) {
        for (auto itr : *(collection.second)) {
            // If parent exists
            if (itr->getParents()) itr->getParents()->addChildren(itr);
        }
    }
}

#ifdef MEMCK
void DEvent::PrintObjectStatistics(const TString& str) {

    std::vector<std::string> Mem_Collections = {"DEvent", "DStep", "McParticle", "SimulatedHit",
                                                "ReconstructedParticle", "CalorimeterHit", "DigiForm"};

    gObjectTable->UpdateInstCount();

    unsigned int n, h, si, ncum = 0, hcum = 0, scum = 0, tcum = 0, thcum = 0;
    Printf("\n[Memory Check] %s", str.Data());
    Printf("class                       count    on heap     size(bytes)    total size(KB)    heap size(KB)");
    Printf("===============================================================================================");
    for (auto cl_name: Mem_Collections) {
        auto cl = gROOT->GetClass(cl_name.data());
        n = cl->GetInstanceCount();
        h = cl->GetHeapInstanceCount();
        si = cl->Size();
        if (n > 0) {
            Printf("%-24s %8d%11d%16d%18.2f%17.2f", cl->GetName(), n, h, si, n * si / 1000., h * si / 1000.);
            ncum += n;
            hcum += h;
            scum += si;
            tcum += n * si;
            thcum += h * si;
        }
    }
    Printf("-----------------------------------------------------------------------------------------------");
    Printf("Total:                   %8d%11d%16d%18.2f%17.2f", ncum, hcum, scum, tcum / 1000., thcum / 1000.);
    Printf("===============================================================================================");
}
#endif

void DEvent::Streamer(TBuffer &R__b)
{
    // Stream an object of class DEvent.

    if (R__b.IsReading()) {
        //std::cout<<"reading event: "<<std::endl;
        //this->Initialization(nALL);

        //DEvent::Class()->
        R__b.ReadClassBuffer(DEvent::Class(),this);
    } else {
        //std::cout<<"writing!!"<<std::endl;
        R__b.WriteClassBuffer(DEvent::Class(),this);
    }
}






