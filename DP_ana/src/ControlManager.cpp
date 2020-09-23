//
// Created by Zhang Yulei on 9/19/20.
//

#include "Core/ControlManager.h"

// System

// Processors
#include "Algo/ExampleProcessor.h"
#include "Algo/RecECAL.h"

void ControlManager::run() {

    auto *evt = new DEvent();
    /* Read in Basic Configuration */
    /* Read Algorithm Lists */
    ConfMgr->ReadConst();

    setFileName(ConfMgr->getInputfile());
    setOutName(ConfMgr->getOutputfile());
    setRunNumber(ConfMgr->getRunNumber());
    setEventNumber(ConfMgr->getEventNumber());
    setSkipNumber(ConfMgr->getSkipNumber());

    EvtReader->setRunNumber(RunNumber);
    EvtReader->setEventNumber(EventNumber);
    EvtReader->setSkipNumber(SkipNumber);

    EvtReader->setVerbose(ConfMgr->getEventReaderVerbose());
    algo->setVerbose(ConfMgr->getAlgoManagerVerbose());
    evt->setVerbose(ConfMgr->getDEventVerbose());

    /* Initialize and Select the AnaProcessors to use*/
    /* Explicitly declare processors with name */
    /* DEFINE ALGO PROCESSOR HERE */
    algo->RegisterAnaProcessor(shared_ptr<ExampleProcessor>(new ExampleProcessor("Example1")) );
    //algo->RegisterAnaProcessor(shared_ptr<ExampleProcessor>(new ExampleProcessor("Example2VeryLongVeryLong")) );
    algo->RegisterAnaProcessor(shared_ptr<RecECAL>(new RecECAL("Example3")) );

    ConfMgr->ReadAlgoList();
    algo->BeginAnaProcessors();

    /*
     *  Readin Config File
     */
    ConfMgr->ReadAnaParameters();

    /*
     *  Begin
     */
    EvtReader->ReadFile(FileName);
    EvtReader->setEvt(evt);

    /*
     *  Processing
     */
    Long64_t nentries = EvtReader->getEntries();
    Long64_t processed_evt = 0;
    if (EventNumber == -1)
        nentries = (nentries >= SkipNumber) ? nentries : SkipNumber;
    else
        nentries = (nentries >= EventNumber + SkipNumber) ? EventNumber + SkipNumber : nentries;
    for (int i = SkipNumber; i < nentries; ++i) {

        cout << "--------------------------";
        cout << " Process Event: " << i;
        cout << " --------------------------" << endl;
        // read the i-th event
        EvtReader->GetEntry(i);

        // convert into DEvent
        EvtReader->Convert();

        // process algorithms
        algo->ProcessEvtAnaProcessors(evt);

        // check algorithms
        algo->CheckEvtAnaProcessors(evt);

        processed_evt++;

        cout << "--------------------------";
        cout << " End of Event:  " << i;
        cout << " --------------------------" << endl;

    }

    /*
     *  End
     */
    algo->EndAnaProcessors();
    algo->PrintRunLog();

    std::cout << std::endl << " ==> Done ..." << std::endl;

    delete evt;
}
