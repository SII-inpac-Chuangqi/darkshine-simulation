//
// Created by Zhang Yulei on 9/19/20.
//

#include "Core/ControlManager.h"

// Processors
#include "Algo/ExampleProcessor.h"

void ControlManager::run() {

    std::cout<<"Start!!"<<std::endl;

    /* Read in Basic Configuration */
    /* Read Algorithm Lists */
    ConfMgr->ReadConst();
    ConfMgr->ReadAlgoList();

    /* Initialize and Select the AnaProcessors to use*/
    /* Explicitly declare processors with name */
    algo->RegisterAnaProcessor(new ExampleProcessor("Example1") );
    algo->RegisterAnaProcessor(new ExampleProcessor("Example2") );
    algo->RegisterAnaProcessor(new ExampleProcessor("Example3") );


    algo->BeginAnaProcessors();

    /*
     *  Readin Config File
     */
    ConfMgr->ReadAnaParameters();

    setFileName(ConfMgr->getInputfile());
    setOutName(ConfMgr->getOutputfile());
    setRunNumber(ConfMgr->getRunNumber());
    setEventNumber(ConfMgr->getEventNumber());
    setSkipNumber(ConfMgr->getSkipNumber());

    /*
     *  Begin
     */
    EvtReader->ReadFile(FileName);
    auto* evt = new DEvent();
    EvtReader->setEvt(evt);

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

    delete evt;
}
