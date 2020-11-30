//
// Created by Zhang Yulei on 11/30/20.
//

#ifndef DSIMU_ECAL_RNN_H
#define DSIMU_ECAL_RNN_H

#include "Core/AnaProcessor.h"

#include "TChain.h"
#include "TMVA/Reader.h"

class ECAL_RNN {
public:
    ECAL_RNN() = default;

    ~ECAL_RNN() = default;

    void TrainDNN(TChain* sig, TChain* bkg, int nb_ch, const string &DSName);

    void LoadModel(int NB_ch, const string &DSPath, const string &DSName);

    double ApplyDNN(const shared_ptr<vector<SimulatedHit *>> &hits);

private:
    TChain *signalTree;
    TChain *background;

    std::string DatasetName;
    int nb_ch;

    std::vector<float> Hits;
    TMVA::Reader* reader;
};


#endif //DSIMU_ECAL_RNN_H
