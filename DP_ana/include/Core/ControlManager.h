//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_CONTROLMANAGER_H
#define DSIMU_CONTROLMANAGER_H

#include "Core/AnaData.h"
#include "Core/AlgoManager.h"
#include "Event/EventReader.h"
#include "Core/ConfigManager.h"
#include "Event/EventStoreAndWriter.h"

#include "TGeoBBox.h"
#include "TGeoManager.h"

class ControlManager {
    /*
     * Control the whole analysis workflow
     */
public:
    ControlManager() = default;

    ~ControlManager() {
        delete EvtReader;
        delete algo;
        delete ConfMgr;
    };

    void run();

    void PrintConfig();



    //

    AlgoManager *getAlgo() const {
        return algo;
    }

    void setAlgo(AlgoManager *in) {
        algo = in;
    }

    int getRunNumber() const {
        return RunNumber;
    }

    void setRunNumber(int runNumber) {
        RunNumber = runNumber;
    }

    int getEventNumber() const {
        return EventNumber;
    }

    void setEventNumber(int eventNumber) {
        EventNumber = eventNumber;
    }

    int getSkipNumber() const {
        return SkipNumber;
    }

    void setSkipNumber(int skipNumber) {
        SkipNumber = skipNumber;
    }

    EventReader *getEvtReader() const {
        return EvtReader;
    }

    void setEvtReader(EventReader *evtReader) {
        EvtReader = evtReader;
    }

    const string &getFileName() const {
        return FileName;
    }

    void setFileName(const string &fileName) {
        FileName = fileName;
    }

    ConfigManager *getConfMgr() const {
        return ConfMgr;
    }

    void setConfMgr(ConfigManager *confMgr) {
        ConfMgr = confMgr;
    }

    const string &getOutName() const {
        return OutName;
    }

    void setOutName(const string &outName) {
        OutName = outName;
    }

    void setOnlyPrintUsage(bool onlyPrintUsage) {
        Only_PrintUsage = onlyPrintUsage;
    }

    /*
     * Read geometry
     */
    void readGeometryDetails();
    void printGeometryDetails() const;

    int    getStripNoTag()     const {return strip_no_tag;}
    double getStripWidthTag()  const {return strip_width_tag;}
    double getStripLengthTag() const {return strip_length_tag;}
    std::vector<double> getAnglesTag() const {return angles_tag;}

    int    getStripNoRec()     const {return strip_no_rec;}
    double getStripWidthRec()  const {return strip_width_rec;}
    double getStripLengthRec() const {return strip_length_rec;}
    std::vector<double> getAnglesRec() const {return angles_rec;}

    int getNECalCellX() const {return N_ECal_cell_x;}
    int getNECalCellY() const {return N_ECal_cell_y;}
    int getNECalCellZ() const {return N_ECal_cell_z;}

private:
    int RunNumber{0};
    int EventNumber{-1};
    int SkipNumber{0};

    std::string FileName;
    std::string OutName;

    AlgoManager *algo{};
    EventReader *EvtReader{};
    ConfigManager *ConfMgr{};

    bool Only_PrintUsage = false;

    TGeoNode* world_{nullptr};

    double strip_width_tag{-INFINITY};
    double strip_length_tag{-INFINITY};
    int strip_no_tag{-1};
    std::vector<double> angles_tag;

    double strip_width_rec{-INFINITY};
    double strip_length_rec{-INFINITY};
    int strip_no_rec{-1};
    std::vector<double> angles_rec;

    int N_ECal_cell_x{0};
    int N_ECal_cell_y{0};
    int N_ECal_cell_z{0};

};


#endif //DSIMU_CONTROLMANAGER_H
