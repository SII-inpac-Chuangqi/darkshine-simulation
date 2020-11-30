//
// Created by Zhang Yulei on 11/30/20.
//

#ifndef DSIMU_ECAL_WRITER_H
#define DSIMU_ECAL_WRITER_H

#include "vector"
#include "Core/AnaProcessor.h"

class ECAL_Writer {
public:
    ECAL_Writer() = default;

    virtual ~ECAL_Writer() = default;

    void BookTree(const TString& filename, const TString& treename, int nb_channel);

    void FillHits(const shared_ptr<vector<SimulatedHit *> >& hits);

    void SaveTree();

private:
    shared_ptr<TFile> outFile;
    shared_ptr<TTree> outTree;
    std::vector<float> ECAL_Hits;
};


#endif //DSIMU_ECAL_WRITER_H
