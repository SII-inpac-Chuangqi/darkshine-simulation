//
// Created by Zhang Yulei on 9/24/20.
//

#ifndef DSIMU_EVENTSTOREANDWRITER_H
#define DSIMU_EVENTSTOREANDWRITER_H

#include <string>
#include <variant>
#include <tuple>
#include <vector>
#include <map>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"

#include "Event/AnaEvent.h"

using AnaVar = std::variant<bool*, short*, int*, float*, double*, std::string*,
                            std::vector<bool>*, std::vector<short>*, std::vector<int>*, std::vector<float>*, std::vector<double>*, std::vector<std::string>*,
                            std::vector<std::vector<bool>>*, std::vector<std::vector<short>>*, std::vector<std::vector<int>>*, std::vector<std::vector<float>>*, std::vector<std::vector<double>>*, std::vector<std::vector<std::string>>*>;

class EventStoreAndWriter {
    /*
     * EventWriter:
     *      Write all the wanted variables to ROOT file
     */
public:
    // Constructors
    EventStoreAndWriter() = default;

    explicit EventStoreAndWriter(std::string outputFileName);

    ~EventStoreAndWriter() {
        //delete tout;
        //delete fout;
    };

    // Get Methods
    const std::string &getOutputFileName() const {
        return OutputFileName;
    }

    const std::string &getTreeName() const {
        return TreeName;
    }

    int getVerbose() const {
        return Verbose;
    }

    [[nodiscard]] TTree *getTout() const {
        return tout;
    }

    // Set Methods
    void setOutputFileName(const std::string &outputFileName) {
        OutputFileName = outputFileName;
    }

    void setTreeName(const std::string &treeName) {
        TreeName = treeName;
    }

    void setVerbose(int verbose) {
        Verbose = verbose;
    }

    // Print Methods
    void PrintTree();

    // Fill Methods
    void FillTree(AnaEvent *);

    void Initialization();

    void CloseFile();

    // Register Methods
    void RegisterTree(const std::string &treename = "dp");

//    template<class data_type>
//    void RegisterOutVariable(const std::string &VarName, data_type *address, const std::string &LeafType = "") {
//        std::cerr << "[RegisterOutVariable] ==> The variable will not registered in CutFlow. " << std::endl;
//
//        if (std::find(RegisteredBranch.begin(), RegisteredBranch.end(), VarName) != RegisteredBranch.end()) {
//            std::cerr << "[WARNING] ==> Variable " << VarName << " has already been registered." << std::endl;
//        } else {
//            if (LeafType.empty())
//                tout->Branch(VarName.c_str(), address);
//            else
//                tout->Branch(VarName.c_str(), address, LeafType.c_str());
//
//            RegisteredBranch.push_back(VarName);
//        }
//    }

    template<class T>
    void RegisterOutVariable(const std::string &var_name, T *address, const std::string &leaf_type = "") {
        //std::cerr << "[RegisterOutVariable] ==> The variable will not registered in CutFlow. " << std::endl;
        std::cerr << "[RegisterOutVariable] ==> Register variable " << var_name << (leaf_type.empty() ? "" : " as " + leaf_type) << std::endl;

        if (std::find(RegisteredBranch.begin(), RegisteredBranch.end(), var_name) != RegisteredBranch.end()) {
            std::cerr << "[WARNING] ==> Variable " << var_name << " has already been registered." << std::endl;
        } else {
            AnaVar ana_var_address = address;
            std::tuple<std::string, AnaVar> ana_var_tuple(leaf_type, ana_var_address);
            ana_var_col.insert(std::pair<std::string, std::tuple<std::string, AnaVar>>(var_name, ana_var_tuple));

            if (leaf_type.empty())
                tout->Branch(var_name.c_str(), address);
            else
                tout->Branch(var_name.c_str(), address, leaf_type.c_str());

            RegisteredBranch.push_back(var_name);
        }
    }

    void RegisterIntVariable(const std::string &VarName, int *address, const std::string &LeafType);

    void RegisterDoubleVariable(const std::string &VarName, double *address, const std::string &LeafType);

    void RegisterStrVariable(const std::string &VarName, TString *address);

    void SaveObjectToFile(TObject *o, const TString &name);

    // Find method
    int *FindIntVar(const std::string &name) {
        if (IntVariables.count(name) != 0) return IntVariables.at(name).second;
        return nullptr;
    };

    double *FindDoubleVar(const std::string &name){
        if (DoubleVariables.count(name) != 0) return DoubleVariables.at(name).second;
        return nullptr;
    };

    TString *FindStrVar(const std::string &name){
        if (StringVariables.count(name) != 0) return StringVariables.at(name);
        return nullptr;
    };

    template<class T>
    T* FindOutVariable(const std::string &var_name) {
        if (std::find(RegisteredBranch.begin(), RegisteredBranch.end(), var_name) == RegisteredBranch.end()) {
            std::cerr << "[WARNING] ==> Variable " << var_name << " not found" << std::endl;
            return nullptr;
        }

        return std::get<T*>(std::get<1>(ana_var_col.at(var_name)));
    }


private:

    // Verbosity
    int Verbose{0};

    // File Name and Tree Name
    std::string OutputFileName;
    std::string TreeName{"dp"};

    // File and Tree components
    TFile *fout{nullptr};
    TTree *tout{nullptr};

    // Registered Variables
    std::map<std::string, std::pair<std::string, int *> > IntVariables;
    std::map<std::string, std::pair<std::string, double *> > DoubleVariables;
    std::map<std::string, TString *> StringVariables;
    std::map<std::string, std::tuple<std::string, AnaVar>> ana_var_col;

    std::vector<std::string> RegisteredBranch;
};


#endif //DSIMU_EVENTSTOREANDWRITER_H
