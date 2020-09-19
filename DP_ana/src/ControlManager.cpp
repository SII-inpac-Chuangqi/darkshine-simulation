//
// Created by Zhang Yulei on 9/19/20.
//

#include "Core/ControlManager.h"

void ControlManager::run() {

    std::cout<<"Start!!"<<std::endl;

    /*
     *  Begin
     */
    EvtReader->ReadFile("dp_out.root");
    auto* evt = new DEvent();
    EvtReader->setEvt(evt);

    algo->BeginAnaProcessors();

    /*
     *  Processing
     */
    Long64_t nentries = EvtReader->getEntries();
    std::cout<<"[READFILE] ==> File with total "+to_string(nentries)+" event(s)."<<std::endl;
    std::cout<<"[READFILE] ==> Skip First "+to_string(SkipNumber)<<" event(s)."<<std::endl;

    if (EventNumber == -1 )
        nentries = (nentries >= SkipNumber) ? nentries : SkipNumber ;
    else
        nentries = (nentries >= EventNumber+SkipNumber) ? EventNumber+SkipNumber : nentries ;
    for (int i = SkipNumber; i < nentries; ++i) {

        // read the i-th event
        EvtReader->GetEntry(i);

        // convert into DEvent
        EvtReader->Convert();

        // process algorithms
        algo->ProcessEvtAnaProcessors(evt);

        // check algorithms
        algo->CheckEvtAnaProcessors(evt);
    }


    /*
     *  End
     */
    algo->EndAnaProcessors();
}
