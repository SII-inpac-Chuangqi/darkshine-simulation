//
// Created by Zhang Yulei on 9/19/20.
//

#include "Core/AlgoManager.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <ctime>
#include <algorithm>

void AlgoManager::RegisterAnaProcessor(std::shared_ptr<AnaProcessor> AnaP) {
    if (AnaProcessors.count(AnaP->getName()) != 0)
        std::cerr << "[WARNING] ==> Algo Processor Name already exists." << std::endl;
    else
        AnaProcessors.insert(std::pair<std::string, std::shared_ptr<AnaProcessor> >(AnaP->getName(), AnaP));
}

void AlgoManager::BeginAnaProcessors() {
    Processed_Evt = 0;

    if (Verbose > 0) {
        cout << "======================================================================" << endl;
        cout << "[ ALGO PROCESS LIST ] : (Verbosity 1)" << endl;
        cout << right;
        cout << setw(10) << "----" << setw(50) << "-----------" << endl;
        cout << setw(10) << "name" << setw(50) << "Description" << endl;
        cout << setw(10) << "----" << setw(50) << "-----------" << endl;
    }

    for (auto itr = AnaProcessorList.begin(); itr != AnaProcessorList.end(); itr++) {
        if (AnaProcessors.count(*itr) == 0) {
            itr = AnaProcessorList.erase(itr);
        }

        // Initialize Processor
        AnaProcessors.at(*itr)->Begin();

        // Initialize Timer
        processing_avg_time.insert(std::pair<std::string, double>(*itr, 0.));

        if (Verbose > 0) {
            cout << right << setw(5) << " " << left;
            cout << setw(39) << *itr;
            cout << setw(50) << AnaProcessors.at(*itr)->getDescription() << endl;
        }
    }

}

void AlgoManager::ProcessEvtAnaProcessors(AnaEvent *evt) {
    for (const auto &itr : AnaProcessorList) {

        // record start time for each processor
        start_processing = clock();

        if (Verbose > 2) {
            cout << "[ PROCESSOR ] (Verbosity 3) : " << itr << endl;
        }
        // process evt
        AnaProcessors.at(itr)->ProcessEvt(evt);

        // record end time for each processor
        end_processing = clock();

        // calculate average processing time (second)
        processing_avg_time.at(itr) = processing_avg_time.at(itr) * static_cast<double>(Processed_Evt) +
                                      double(end_processing - start_processing) / CLOCKS_PER_SEC;

        processing_avg_time.at(itr) /= static_cast<double>(Processed_Evt + 1);
    }

    Processed_Evt++;
}

void AlgoManager::CheckEvtAnaProcessors(AnaEvent *evt) {
    for (const auto &itr : AnaProcessorList)
        AnaProcessors.at(itr)->CheckEvt(evt);
}

void AlgoManager::EndAnaProcessors() {
    for (const auto &itr : AnaProcessorList)
        AnaProcessors.at(itr)->End();

}

AnaProcessorVecUniPtr AlgoManager::getAllAnaProcessors() {
    AnaProcessorVecUniPtr tmp(new AnaProcessorVec());
    for (const auto &itr : AnaProcessors)
        tmp->emplace_back(itr.second);

    return tmp;
}

void AlgoManager::SetAnaProcessorsList(const std::string &ProcessorList) {

    istringstream sin(ProcessorList);
    do {
        std::string ProcessorName;
        sin >> ProcessorName;

        if (!ProcessorName.empty()) {
            if (std::find(AnaProcessorList.begin(), AnaProcessorList.end(), ProcessorName) == AnaProcessorList.end()) {
                {
                    if (AnaProcessors.count(ProcessorName) != 0)
                        AnaProcessorList.emplace_back(ProcessorName);
                    else
                        std::cerr << "[WARNING] ==> No Algo Processor named: " << ProcessorName << std::endl;
                }
            } else
                std::cerr << "[WARNING] ==> Duplicate Algo Processor Name." << std::endl;
        }
    } while (sin);
}

void AlgoManager::PrintRunLog() {
    cout << "======================================================================" << endl;
    cout << "---------------------------> Run Summary <----------------------------" << endl;

    double total_time = 0.;

    cout << left;
    cout << setw(5) << " " << setw(35) << "Processor Name";
    cout << setw(40) << "Execution Time / Event [sec]" << endl;
    cout << fixed << setprecision(8);
    for (const auto &itr : AnaProcessorList) {

        cout << setw(5) << " " << setw(40) << itr;
        cout << setw(40) << processing_avg_time.at(itr) << endl;

        total_time += processing_avg_time.at(itr);
    }
    cout << "----------------------------------------------------------------------" << endl;
    cout << setw(5) << " " << setw(26) << "Total Processed Event(s): " << setw(14) << Processed_Evt;
    cout << setw(40) << total_time << endl;
    cout << "======================================================================" << endl;
}
