//
// Created by Zhang Yulei on 9/24/20.
//

#ifndef DSIMU_EVENTSTOREANDWRITER_H
#define DSIMU_EVENTSTOREANDWRITER_H

#include <string>
#include <map>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"

#include "Event/AnaEvent.h"

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
    } ;

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
    void FillTree(AnaEvent* );
    void Initialization();
    void CloseFile();

    // Register Methods
    void RegisterTree(const std::string& treename = "dp");

    void RegisterIntVariable(const std::string& VarName, int *address, const std::string& LeafType);
    void RegisterDoubleVariable(const std::string& VarName, double *address, const std::string& LeafType);
    void RegisterStrVariable(const std::string& VarName, TString *address);

private:

    // Verbosity
    int Verbose{0};

    // File Name and Tree Name
    std::string OutputFileName;
    std::string TreeName{"dp"};

    // File and Tree components
    TFile* fout{nullptr};
    TTree* tout{nullptr};

    // Registered Variables
    std::map<std::string, std::pair<std::string, int *> > IntVariables;
    std::map<std::string, std::pair<std::string, double *> > DoubleVariables;
    std::map<std::string, TString *>  StringVariables;
};


#endif //DSIMU_EVENTSTOREANDWRITER_H
