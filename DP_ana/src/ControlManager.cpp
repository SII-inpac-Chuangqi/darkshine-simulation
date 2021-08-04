//
// Created by Zhang Yulei on 9/19/20.
//

#include "Core/ControlManager.h"

// System
#include "Event/AnaEvent.h"

// Processors
#include "Algo/ExampleProcessor.h"
#include "Algo/MCTruthAnalysis.h"
#include "Algo/RecECAL.h"
#include "Algo/Digitizer.h"

#ifndef _OFF_TRACKING

#include "Algo/TrackingProcessor.h"

#endif

#include "Algo/CutFlowAnalysis.h"

void ControlManager::run() {

    /* Read in Basic Configuration */
    /* Read Algorithm Lists */
    if (ConfMgr) {
        ConfMgr->ReadConst();

        setFileName(ConfMgr->getInputfile());
        setOutName(ConfMgr->getOutputfile());
        setRunNumber(ConfMgr->getRunNumber());
        setEventNumber(ConfMgr->getEventNumber());
        setSkipNumber(ConfMgr->getSkipNumber());
    }
    // Define output and data storage
    std::shared_ptr<EventStoreAndWriter> EvtWrt = Only_PrintUsage ? nullptr : make_shared<EventStoreAndWriter>(OutName);

    EvtReader->setRunNumber(RunNumber);
    EvtReader->setEventNumber(EventNumber);
    EvtReader->setSkipNumber(SkipNumber);
    EvtReader->setEvtWrt(EvtWrt);

    // Set Verbosity
    if (ConfMgr) {
        EvtReader->setVerbose(ConfMgr->getEventReaderVerbose());
        algo->setVerbose(ConfMgr->getAlgoManagerVerbose());
        //evt->setVerbose(ConfMgr->getDEventVerbose());
        EvtWrt->setVerbose(ConfMgr->getEventStoreAndWriterVerbose());

        // Register Output Tree
        EvtWrt->RegisterTree();
        EvtReader->RegisterOutput();
    }

    // Read Geometry from ROOT file
    if (!Only_PrintUsage) {
        EvtReader->ReadFile(FileName);

        EvtReader->ReadGeometry(ConfMgr->getInputGeofile());
        dAnaData->setRootFile(EvtReader->getDataFile());
    }

    /* Initialize and Select the AnaProcessors to use*/
    /* Explicitly declare processors with name */
    /* DEFINE ALGO PROCESSOR HERE */
    algo->RegisterAnaProcessor(shared_ptr<Digitizer>(new Digitizer("Digitizer", EvtWrt)));
    algo->RegisterAnaProcessor(shared_ptr<MCTruthAnalysis>(new MCTruthAnalysis("MCTruthAnalysis", EvtWrt)));
    algo->RegisterAnaProcessor(shared_ptr<RecECAL>(new RecECAL("RecECAL", EvtWrt)));
#ifndef _OFF_TRACKING
    algo->RegisterAnaProcessor(shared_ptr<TrackingProcessor>(new TrackingProcessor("Tracking", EvtWrt)));
#endif
    algo->RegisterAnaProcessor(shared_ptr<CutFlowAnalysis>(new CutFlowAnalysis("CutFlowAnalysis", EvtWrt)));

    if (ConfMgr) {
        ConfMgr->ReadAlgoList();
        /*
        *  Readin Config File
        */
        ConfMgr->ReadAnaParameters();
    }

    /*
     *  Begin
     */
    algo->BeginAnaProcessors();

    if (Only_PrintUsage) {
        PrintConfig();
        return;
    }

    // Print Output Tree
    EvtWrt->PrintTree();

    /*
     *  Processing
     */
    AnaEvent *evt;
    Long64_t nentries = EvtReader->getEntries();
    Long64_t processed_evt = 0;
    if (EventNumber == -1)
        nentries = (nentries >= SkipNumber) ? nentries : SkipNumber;
    else
        nentries = (nentries >= EventNumber + SkipNumber) ? EventNumber + SkipNumber : nentries;
    for (int i = 0; i < nentries; ++i) {
        // read the i-th event
        // Skip events
        if (i < SkipNumber) continue;

        if (ConfMgr->getEventReaderVerbose() > 1) {
            cout << "--------------------------";
            cout << " Process Event: " << i;
            cout << " --------------------------" << endl;
        }

        //evt->Initialization(nALL);

#ifdef MEMCK
        if (ConfMgr->getMemoryCheckVerbose() > 1)
            AnaEvent::PrintObjectStatistics(Form("Begin of Event: %d", i));
#endif
        // convert into DEvent
        EvtReader->ReadEntry(i);
        EvtReader->Convert();

#ifdef MEMCK
        if (ConfMgr->getMemoryCheckVerbose() > 2)
            AnaEvent::PrintObjectStatistics("Read DEvent from ROOT");
#endif
        evt = EvtReader->getEvt();

        // process algorithms
        algo->ProcessEvtAnaProcessors(evt, processed_evt);

        // check algorithms
        algo->CheckEvtAnaProcessors(evt);

        // Fill Output
        EvtWrt->FillTree(evt);

        processed_evt++;

        if (ConfMgr->getEventReaderVerbose() > 1) {
            cout << "--------------------------";
            cout << " End of Event:  " << i;
            cout << " --------------------------" << endl;
        }

        evt->Initialization(nALL);

#ifdef MEMCK
        if (ConfMgr->getMemoryCheckVerbose() > 0)
            AnaEvent::PrintObjectStatistics(Form("End of Event: %d", i));
#endif
    }

    /*
     *  End
     */
    algo->EndAnaProcessors();
    algo->PrintRunLog();

    // Close Output File
    EvtWrt->CloseFile();

    std::cout << std::endl << " ==> Done ..." << std::endl;
}

void ControlManager::PrintConfig() {
    using namespace std;

    // Print header
    cout << "############################" << endl;
    cout << "###" << endl;
    cout << "### Example Config File" << endl;
    cout << "###" << endl;
    cout << "############################" << endl << endl;

    cout << "### Basic Settings" << endl << left;
    cout << setw(15) << "InputFile" << "= dp_out.root" << endl;
    cout << setw(15) << "InputGeoFile" << "= dp_out.root" << endl;
    cout << setw(15) << "OutputFile" << "= dp_ana.root" << endl;
    cout << setw(15) << "RunNumber" << "= 0" << endl;
    cout << setw(15) << "EventNumber" << "= -1" << endl;
    cout << setw(15) << "SkipNumber" << "= 0" << endl;

    cout << endl << "### Verbosity Settings" << endl << left;
    cout << setw(30) << "AlgoManager.Verbose" << "= 0" << endl;
    cout << setw(30) << "EventReader.Verbose" << "= 0" << endl;
    cout << setw(30) << "EventStoreAndWriter.Verbose" << "= 0" << endl;
    cout << setw(30) << "MemoryCheck.Verbose" << "= 0" << endl;

    // Print Algorithm List
    cout << endl << "### Algorithm List" << endl << left;
    cout << "###" << endl;
    for (const auto &pl : algo->getAnaProcessorListDefault()) {
        auto p = algo->getAnaProcessors().at(pl);
        cout << "# " << pl << ": " << p->getDescription() << endl;
    }
    cout << "###" << endl;
    cout << "Algorithm.List" << " = ";
    for (const auto &pl : algo->getAnaProcessorListDefault())
        cout << pl << " ";
    cout << endl;

    // Print Algorithm Parameters
    cout << endl << "### Algorithm Configuration" << endl << left;
    for (const auto &p : algo->getAnaProcessors()) {
        cout << endl;
        for (const auto &para : p.second->getIntParameters())
            cout << p.first << "." << para.first << " = " << *(para.second.second) << "  # " << para.second.first
                 << endl;
        for (const auto &para : p.second->getDoubleParameters())
            cout << p.first << "." << para.first << " = " << *(para.second.second) << "  # " << para.second.first
                 << endl;
        for (const auto &para : p.second->getStringParameters())
            cout << p.first << "." << para.first << " = " << *(para.second.second) << "  # " << para.second.first
                 << endl;
    }
}
