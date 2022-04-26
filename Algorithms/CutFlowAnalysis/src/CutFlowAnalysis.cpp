//
// Created by Zhang Yulei on 9/19/20.
//

#include "Algo/CutFlowAnalysis.h"

#include <utility>
#include <algorithm>

#include <string>
#include <sstream>

CutFlowAnalysis::CutFlowAnalysis(string name, shared_ptr<EventStoreAndWriter> evtwrt)
        : AnaProcessor(std::move(name), std::move(evtwrt)) {

    // Add description for this AnaProcessor
    // Description = "Automatically Generate Cut Flow.";
}


void CutFlowAnalysis::Begin() {
      weight = EvtWrt->FindOutVariable<double>("weight");
      //weight = EvtWrt->FindDoubleVar("weight");
      ECal_seed_x = EvtWrt->FindOutVariable<std::vector<double>>("ECal_seed_x");
}

void CutFlowAnalysis::ProcessEvt(AnaEvent* /*evt*/) {
      if(weight) std::cout << *weight << ":\t";
      if(ECal_seed_x) for(auto seed : *ECal_seed_x) std::cout << seed << "\t";
      std::cout << std::endl;
}

void CutFlowAnalysis::CheckEvt(AnaEvent* /*evt*/) {
    //cout<<"Check!"<<endl;

}

void CutFlowAnalysis::End() {
}
