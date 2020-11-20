//
// Created by Zhang Yulei on 9/19/20.
//

#include "Core/ControlManager.h"

// System

// Processors
#include "Algo/ExampleProcessor.h"
#include "Event/AnaEvent.h"
#include "Algo/MCTruthAnalysis.h"
#include "Algo/RecECAL.h"

void ControlManager::run() {

    auto *evt = new AnaEvent();
    /* Read in Basic Configuration */
    /* Read Algorithm Lists */
    ConfMgr->ReadConst();

    setFileName(ConfMgr->getInputfile());
    setOutName(ConfMgr->getOutputfile());
    setRunNumber(ConfMgr->getRunNumber());
    setEventNumber(ConfMgr->getEventNumber());
    setSkipNumber(ConfMgr->getSkipNumber());

    // Define output and data storage
    std::shared_ptr<EventStoreAndWriter> EvtWrt = make_shared<EventStoreAndWriter>(OutName);

    EvtReader->setRunNumber(RunNumber);
    EvtReader->setEventNumber(EventNumber);
    EvtReader->setSkipNumber(SkipNumber);
    EvtReader->setEvtWrt(EvtWrt);

    // Set Verbose
    EvtReader->setVerbose(ConfMgr->getEventReaderVerbose());
    algo->setVerbose(ConfMgr->getAlgoManagerVerbose());
    evt->setVerbose(ConfMgr->getDEventVerbose());
    EvtWrt->setVerbose(ConfMgr->getEventStoreAndWriterVerbose());

    // Register Output Tree
    EvtWrt->RegisterTree();
    EvtReader->RegisterOutput();

    /* Initialize and Select the AnaProcessors to use*/
    /* Explicitly declare processors with name */
    /* DEFINE ALGO PROCESSOR HERE */
    //algo->RegisterAnaProcessor(shared_ptr<ExampleProcessor>(new ExampleProcessor("Example1", EvtWrt)) );
    algo->RegisterAnaProcessor(shared_ptr<MCTruthAnalysis>(new MCTruthAnalysis("MCTruthAnalysis", EvtWrt)) );
    algo->RegisterAnaProcessor(shared_ptr<RecECAL>(new RecECAL("RecECAL", EvtWrt)) );

    ConfMgr->ReadAlgoList();
    algo->BeginAnaProcessors();

    // Print Output Tree
    EvtWrt->PrintTree();

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
    for (int i = 0; i < nentries; ++i) {
        // read the i-th event
        if ( !EvtReader->ReadNextEntry() ) break;

        // Skip events
        if ( i < SkipNumber ) continue;

        cout << "--------------------------";
        cout << " Process Event: " << i;
        cout << " --------------------------" << endl;

        // convert into DEvent
        EvtReader->Convert();

        // process algorithms
        algo->ProcessEvtAnaProcessors(evt);

        // check algorithms
        algo->CheckEvtAnaProcessors(evt);

        // Fill Output
        EvtWrt->FillTree(evt);

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

    // Close Output File
    EvtWrt->CloseFile();

    std::cout << std::endl << " ==> Done ..." << std::endl;

    delete evt;
}
