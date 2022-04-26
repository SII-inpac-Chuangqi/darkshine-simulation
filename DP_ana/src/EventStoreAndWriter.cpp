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

void EventStoreAndWriter::RegisterIntVariable(const std::string &var_name, int *address, const std::string &leaf_type) {
//    std::cerr<<"[RegisterIntVariable] ==> This method will be deprecated soon. "<<std::endl;
//    std::cerr<<"                          Please use the new method: RegisterOutVariable() "<<std::endl;

    if (int_variables_.count(var_name) != 0) {
        std::cerr << "[WARNING] ==> Int variable " << var_name << " already exists." << std::endl;
    } else {
        std::pair<std::string, int *> int_pair("int", address);
        int_variables_.insert(std::pair<std::string, std::pair<std::string, int *> >(var_name, int_pair));
        ana_var_col_.insert(std::pair<std::string, std::pair<std::string, AnaVar>>(var_name, int_pair));

        tout->Branch(var_name.c_str(), address, leaf_type.c_str());
        registered_branch_.push_back(var_name);
    }
}

void EventStoreAndWriter::RegisterDoubleVariable(const std::string &var_name, double *address, const std::string &leaf_type) {
//    std::cerr<<"[RegisterDoubleVariable] ==> This method will be deprecated soon. "<<std::endl;
//    std::cerr<<"                             Please use the new method: RegisterOutVariable() "<<std::endl;

    if (double_variables_.count(var_name) != 0) {
        std::cerr << "[WARNING] ==> Double variable " << var_name << " already exists." << std::endl;
    } else {
        std::pair<std::string, double *> double_pair("double", address);
        double_variables_.insert(std::pair<std::string, std::pair<std::string, double *> >(var_name, double_pair));
        ana_var_col_.insert(std::pair<std::string, std::pair<std::string, AnaVar>>(var_name, double_pair));

        tout->Branch(var_name.c_str(), address, leaf_type.c_str());
        registered_branch_.push_back(var_name);
    }
}

void EventStoreAndWriter::RegisterStrVariable(const std::string &var_name, TString *address) {
//    std::cerr<<"[RegisterStrVariable] ==> This method will be deprecated soon. "<<std::endl;
//    std::cerr<<"                          Please use the new method: RegisterOutVariable() "<<std::endl;

    if (str_variables_.count(var_name) != 0) {
        std::cerr << "[WARNING] ==> String variable " << var_name << " already exists." << std::endl;
    } else {
        std::pair<std::string, TString *> str_pair("string", address);
        str_variables_.insert(std::pair<std::string, TString *>(var_name, address));
        ana_var_col_.insert(std::pair<std::string, std::pair<std::string, AnaVar>>(var_name, str_pair));

        tout->Branch(var_name.c_str(), address);
        registered_branch_.push_back(var_name);
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
        for (const auto &itr : int_variables_) {
            std::cout << std::setw(5) << " " << std::setw(40);
            std::cout << "==> Registered Int Variable: " << std::setw(30) << itr.first << std::endl;
        }
        for (const auto &itr : double_variables_) {
            std::cout << std::setw(5) << " " << std::setw(40);
            std::cout << "==> Registered Double Variable: " << std::setw(30) << itr.first << std::endl;
        }
        for (const auto &itr : str_variables_) {
            std::cout << std::setw(5) << " " << std::setw(40);
            std::cout << "==> Registered String Variable: " << std::setw(30) << itr.first << std::endl;
        }
        for (const auto &itr : registered_branch_) {
            std::cout << std::setw(5) << " " << std::setw(40);
            std::cout << "==> Registered Out Branch: " << std::setw(30) << itr << std::endl;
        }
    }
}

void EventStoreAndWriter::FillTree(AnaEvent* /*Evt*/) {

    tout->Fill();
    Initialization();
}

void EventStoreAndWriter::CloseFile() {
    if (fout) {
        fout->cd();
        tout->Write("", TObject::kOverwrite);
        fout->Close();
    }

    int_variables_.clear();
    double_variables_.clear();
    str_variables_.clear();
    ana_var_col_.clear();

    registered_branch_.clear();
}

void EventStoreAndWriter::SaveObjectToFile(TObject* o, const TString& name) {
    if (fout) {
        fout->cd();
        o->Write(name, TObject::kOverwrite);
    }
}


