//
// Created by Zhang Yulei on 9/24/20.
//

#include "Event/EventStoreAndWriter.h"

#include <utility>
#include <iostream>
#include <iomanip>

using namespace std;

EventStoreAndWriter::EventStoreAndWriter(std::string outputFileName) : OutputFileName(std::move(outputFileName)) {
    fout = new TFile(OutputFileName.c_str(), "RECREATE");
    if (!fout)
        std::cerr << "[ERROR] Fail to create ROOT file: " << OutputFileName << std::endl;

    tout = nullptr;
}


void EventStoreAndWriter::Initialization() {

}

void EventStoreAndWriter::RegisterTree(const std::string &treename) {
    TreeName = treename;

    fout->cd();
    tout = new TTree(TreeName.c_str(), TreeName.c_str());
}

void EventStoreAndWriter::RegisterIntVariable(const std::string &VarName, int *address, const std::string &LeafType) {
//    std::cerr<<"[RegisterIntVariable] ==> This method will be deprecated soon. "<<std::endl;
//    std::cerr<<"                          Please use the new method: RegisterOutVariable() "<<std::endl;

    if (IntVariables.count(VarName) != 0) {
        std::cerr << "[WARNING] ==> Int Variable " << VarName << " already exists." << std::endl;
    } else {
        std::pair<std::string, int *> tmp(LeafType, address);
        IntVariables.insert(std::pair<std::string, std::pair<std::string, int *> >(VarName, tmp));

        tout->Branch(VarName.c_str(), address, LeafType.c_str());
    }
}

void EventStoreAndWriter::RegisterDoubleVariable(const string &VarName, double *address, const string &LeafType) {
//    std::cerr<<"[RegisterDoubleVariable] ==> This method will be deprecated soon. "<<std::endl;
//    std::cerr<<"                             Please use the new method: RegisterOutVariable() "<<std::endl;

    if (DoubleVariables.count(VarName) != 0) {
        std::cerr << "[WARNING] ==> double Variable " << VarName << " already exists." << std::endl;
    } else {
        std::pair<std::string, double *> tmp(LeafType, address);
        DoubleVariables.insert(std::pair<std::string, std::pair<std::string, double *> >(VarName, tmp));

        tout->Branch(VarName.c_str(), address, LeafType.c_str());
    }
}

void EventStoreAndWriter::RegisterStrVariable(const string &VarName, TString *address) {
//    std::cerr<<"[RegisterStrVariable] ==> This method will be deprecated soon. "<<std::endl;
//    std::cerr<<"                          Please use the new method: RegisterOutVariable() "<<std::endl;

    if (StringVariables.count(VarName) != 0) {
        std::cerr << "[WARNING] ==> String Variable " << VarName << " already exists." << std::endl;
    } else {
        //std::pair<std::string, TString *> tmp(LeafType, address);
        StringVariables.insert(std::pair<std::string, TString *>(VarName, address));

        tout->Branch(VarName.c_str(), address);
    }
}

void EventStoreAndWriter::PrintTree() {
    if (Verbose > 0) {
        cout << "======================================================================" << endl;
        std::cout << "[EVENT READER] (Verbosity 1) : RegisterTree" << std::endl;
        std::cout << left;
        std::cout << std::setw(5) << " " << std::setw(30);
        std::cout << "==> Output File: " << std::setw(30) << OutputFileName << std::endl;

        std::cout << std::setw(5) << " " << std::setw(30);
        std::cout << "==> Registered Tree Name: " << std::setw(30) << TreeName << std::endl;

        cout << "----------------------------------------------------------------------" << endl;
    }
    if (Verbose > 1) {
        for (const auto &itr : IntVariables) {
            std::cout << std::setw(5) << " " << std::setw(40);
            std::cout << "==> Registered Int Variable: " << std::setw(30) << itr.first << std::endl;
        }
        for (const auto &itr : DoubleVariables) {
            std::cout << std::setw(5) << " " << std::setw(40);
            std::cout << "==> Registered Double Variable: " << std::setw(30) << itr.first << std::endl;
        }
        for (const auto &itr : StringVariables) {
            std::cout << std::setw(5) << " " << std::setw(40);
            std::cout << "==> Registered String Variable: " << std::setw(30) << itr.first << std::endl;
        }
        for (const auto &itr : RegisteredBranch) {
            std::cout << std::setw(5) << " " << std::setw(40);
            std::cout << "==> Registered Out Branch: " << std::setw(30) << itr << std::endl;
        }
    }
}

void EventStoreAndWriter::FillTree(AnaEvent *Evt) {

    tout->Fill();
    Initialization();
}

void EventStoreAndWriter::CloseFile() {
    if (fout) {
        fout->cd();
        tout->Write("", TObject::kOverwrite);
        fout->Close();
    }

    IntVariables.clear();
    DoubleVariables.clear();
    StringVariables.clear();

    RegisteredBranch.clear();
}

void EventStoreAndWriter::SaveObjectToFile(TObject* o, const TString& name) {
    if (fout) {
        fout->cd();
        o->Write(name, TObject::kOverwrite);
    }
}


