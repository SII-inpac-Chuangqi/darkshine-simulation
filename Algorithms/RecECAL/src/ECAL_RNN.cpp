//
// Created by Zhang Yulei on 11/30/20.
//

#include "Algorithms/RecECAL/include/Algo/ECAL_RNN.h"
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/DataSetInfo.h"

void ECAL_RNN::TrainDNN(TChain *sig, TChain *bkg, int NB_ch, const string &DSName) {
    std::cout << "--- RNNClassification  : " << std::endl;

    signalTree = sig;
    background = bkg;

    // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
    TString outfileName(DSName + ".root");
    TFile *outputFile = TFile::Open(outfileName, "RECREATE");

    // Creating the factory object
    auto *factory = new TMVA::Factory("TMVAClassification", outputFile,
                                      "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");
    auto *dataloader = new TMVA::DataLoader(DSName);

    // add variables (time zero and time 1)
    for (int i = 0; i < NB_ch; ++i) {
        TString varName = TString::Format("ECAL_%d", i);
        dataloader->AddVariable(varName, 'F');
    }

    dataloader->AddSignalTree(signalTree, 1.0);
    dataloader->AddBackgroundTree(background, 1.0);

    // check given input
    auto &datainfo = dataloader->GetDataSetInfo();
    auto vars = datainfo.GetListOfVariables();
    std::cout << "number of variables is " << vars.size() << std::endl;

    int ntrainEvts = (int) (0.8 * min(signalTree->GetEntries(), background->GetEntries()));
    int ntestEvts = (int) (0.25 * ntrainEvts);
    TString trainAndTestOpt = TString::Format(
            "nTrain_Signal=%d:nTrain_Background=%d:nTest_Signal=%d:nTest_Background=%d:SplitMode=Random:NormMode=NumEvents:!V",
            ntrainEvts, ntrainEvts, ntestEvts, ntestEvts);
    TCut mycuts = "";//Entry$<1000"; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
    TCut mycutb = "";//Entry$<1000";
    dataloader->PrepareTrainingAndTestTree(mycuts, mycutb, trainAndTestOpt);

    std::cout << "prepared DATA LOADER " << std::endl;
    // Input Layout
    TString inputLayoutString = TString::Format("InputLayout=1|1|%d",NB_ch);

    // Batch Layout
    TString batchLayoutString("BatchLayout=256|2|300");

    // General layout.
    //TString layoutString("Layout=RNN|128|300|2|0,DENSE|64|TANH,DENSE|2|LINEAR");
    TString layoutString("Layout=DENSE|64|TANH,DENSE|64|TANH,DENSE|64|TANH,LINEAR");

    // Training strategies.
    TString training0("LearningRate=1e-1,Momentum=0.9,Repetitions=1,"
                      "ConvergenceSteps=100,BatchSize=16,TestRepetitions=1,"
                      "WeightDecay=1e-4,Regularization=L2,"
                      "DropConfig=0.0+0.5+0.5+0.5, Multithreading=True");
    TString training1("LearningRate=1e-2,Momentum=0.9,Repetitions=1,"
                      "ConvergenceSteps=20,BatchSize=256,TestRepetitions=10,"
                      "WeightDecay=1e-4,Regularization=L2,"
                      "DropConfig=0.0+0.0+0.0+0.0, Multithreading=True");
    TString training2("LearningRate=1e-3,Momentum=0.0,Repetitions=1,"
                      "ConvergenceSteps=20,BatchSize=256,TestRepetitions=10,"
                      "WeightDecay=1e-4,Regularization=L2,"
                      "DropConfig=0.0+0.0+0.0+0.0, Multithreading=True");
    TString trainingStrategyString("TrainingStrategy=");
    trainingStrategyString += training0; // + "|" + training1 + "|" + training2;

    // General Options.
    TString rnnOptions("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=N:"
                       "WeightInitialization=XAVIERUNIFORM");

    rnnOptions.Append(":");
    rnnOptions.Append(inputLayoutString);
    rnnOptions.Append(":");
    //rnnOptions.Append(batchLayoutString);
    //rnnOptions.Append(":");
    rnnOptions.Append(layoutString);
    rnnOptions.Append(":");
    rnnOptions.Append(trainingStrategyString);
    rnnOptions.Append(":Architecture=CPU");

    factory->BookMethod(dataloader, TMVA::Types::kDL, "DNN_CPU", rnnOptions);

    factory->TrainAllMethods();
}

void ECAL_RNN::LoadModel(int NB_ch, const string &DSPath, const string &DSName) {
    nb_ch = NB_ch;
    DatasetName = DSName;

    reader = new TMVA::Reader();
    for (int i = 0; i < nb_ch; ++i) {
        Hits.push_back(0.);
        TString varName = TString::Format("ECAL_%d", i);
        reader->AddVariable(varName, &Hits.at(i));
    }

    reader->BookMVA(DSName, DSPath);
}

double ECAL_RNN::ApplyDNN(const shared_ptr<vector<SimulatedHit *>> &hits) {
    for (auto hit : *hits) {
        int cell_id = hit->getCellId();
        Hits.at(cell_id-1) = ( hit->getE() > 1e-6 && !isnan(hit->getE()) ) ? hit->getE() : 0. ;
    }
    return reader->EvaluateMVA(DatasetName);
}


