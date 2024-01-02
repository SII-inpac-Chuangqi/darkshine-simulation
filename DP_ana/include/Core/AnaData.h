//
// Created by zhuxu on 2021/7/29.
//

#ifndef DSIMU_ANADATA_H
#define DSIMU_ANADATA_H

#ifdef RM_UNIT
#define CUNIT 1
#else
#define CUNIT 10
#endif
//logic of length unit
// GEANT4 aleays use mm
// TGeo and TEve depends on version, and defined using CUNIT
// When read from TGeo/yaml/TEve x=f() : x*CUNIT = x_mm
// When send to TGeo/TEve f(x) : x_mm/CUNIT = x
// Plain number in DSimu/DAna/DDis always using mm

#ifndef RETURN
#define RETURN std::nan("RETURN")
#endif

#include "yaml-cpp/yaml.h"

#include "TFile.h"
#include "TGeoBBox.h"
#include "TGeoManager.h"
#include "TVector3.h"

#include "Object/DTruth.h"
#include "Object/DMagnet.h"
#include "Object/McPHelper.h"

#include <vector>
#include <tuple>
#include <unordered_map>

using std::vector, std::tuple;

#define ACC(x,y,z) (((x)-1)+N_ECal_cell_x*((y)-1)+N_ECal_cell_x*N_ECal_cell_y*((z)-1)) // posmap start from 0 0 0


class AnaData {
public:
    AnaData(const AnaData &) = delete;

    AnaData &operator=(AnaData const &) = delete;

    static AnaData *CreateInstance();

    void ReadMagField();

    [[nodiscard]] const vector<DMagnet *> &getMagFieldVec() const {
        return mag_field_vec;
    }
    void setConstMagnetField(const vector<double> &const_value);
    [[nodiscard]] const vector<double> getMagnetFieldAt(const vector<double> &pos) const;

    [[nodiscard]] TFile *getRootFile() const {
        return root_file;
    }

    void setRootFile(TFile *rootFile) {
        root_file = rootFile;
        ReadMagField();
    }

//................................................................................//
//Geometry manager
    void readGeometryDetails();
    void printGeometryDetails() const;

//Tag tracker
    const std::vector<double> getLayerWidthTag()  const {return layer_width_tag;}
    const std::vector<double> getLayerLengthTag() const {return layer_length_tag;}
    const std::vector<double> getLayerThicknessTag() const {return layer_thickness_tag;}
    const std::vector<int>    getStripNoTag() const {return strip_no_tag;}
    const std::vector<double> getAnglesTag()  const {return angles_tag;}
    double getCenterXTag() const {return center_x_tag_;}
    double getCenterYTag() const {return center_y_tag_;}
    double getCenterZTag() const {return center_z_tag_;}
    double getLengthXTag() const {return length_x_tag_;}
    double getLengthYTag() const {return length_y_tag_;}
    double getLengthZTag() const {return length_z_tag_;}
    
//Recoil Tracker
    const std::vector<double> getLayerWidthRec()  const {return layer_width_rec;}
    const std::vector<double> getLayerLengthRec() const {return layer_length_rec;}
    const std::vector<double> getLayerThicknessRec() const {return layer_thickness_rec;}
    const std::vector<int>    getStripNoRec() const {return strip_no_rec;}
    const std::vector<double> getAnglesRec()  const {return angles_rec;}

    double getCenterXRec() const {return center_x_rec_;}
    double getCenterYRec() const {return center_y_rec_;}
    double getCenterZRec() const {return center_z_rec_;}
    double getLengthXRec() const {return length_x_rec_;}
    double getLengthYRec() const {return length_y_rec_;}
    double getLengthZRec() const {return length_z_rec_;}

//ECal
    int getECAL_globalID(int block, int unit);
    const std::vector<TVector3>& getECalPosMap() const {return ECAL_posmap;};
    [[maybe_unused]] double getECalCenterX() const {return ECAL_center_x;}
    [[maybe_unused]] double getECalCenterY() const {return ECAL_center_y;}
    [[maybe_unused]] double getECalCenterZ() const {return ECAL_center_z;}
    [[maybe_unused]] double getECalLengthX() const {return ECAL_length_x;}
    [[maybe_unused]] double getECalLengthY() const {return ECAL_length_y;}
    [[maybe_unused]] double getECalLengthZ() const {return ECAL_length_z;}
    [[maybe_unused]] std::vector<double> getECalCellLengthX() const {return ECal_cell_length_x;}
    [[maybe_unused]] std::vector<double> getECalCellLengthY() const {return ECal_cell_length_y;}
    [[maybe_unused]] std::vector<double> getECalCellLengthZ() const {return ECal_cell_length_z;}
    int getNECalCellX() const {return N_ECal_cell_x;}
    int getNECalCellY() const {return N_ECal_cell_y;}
    int getNECalCellZ() const {return N_ECal_cell_z;}
    int getNECalCells() const {return N_ECal_cells;}
    double getECalSurfaceZ() const {return ECAL_center_z - 0.5*ECAL_length_z;}
    const std::vector<std::vector<int>> &getCenterIdNeighborIds_staggered() {return centerIdNeighborIds_staggered;}
    TString getRegionName(const float vertex[3]);

    int getProcessId(const std::string& n);
    void printProcessMap();

    bool makeCenterIdNeighborIdsMap_legacy();
    bool makeCenterIdNeighborIdsMap_staggered();

//................................................................................//
//Truth helper manager
    void LoadTruthInfo(DTruth *truth);
    void PrintTruthInfo() const;
    bool hasDTruth(){return truth_!=nullptr;};

    std::vector<std::pair<const DTruthState*,int>> getTruthStatesAtECalFront() const; // for tracker
    std::vector<std::pair<const DTruthParticle*, const DTruthState*>> getTruthsAtECalFront() const; // for ECAL

    unsigned int getNTruthTracks(DTruth::DTruthDetPV DetPV, double min_energy = 50., int min_hits = 4) const;
    std::map<pair<int, int>, vector<DTruthState *>> getTruthTracks(DTruth::DTruthDetPV DetPV, double min_energy = 50., int min_hits = 4) const;

    const DTruth* getInitialElectron() const;

    //const McPHelper* getInitialElectron() const;
    //void PrintTruthMcPHelper() const;
    //void LoadTruthMcPHelper(const MCPHelperMap &helper_collection);

protected:
    TFile* root_file;

    bool if_const_field_{false};
    vector<DMagnet*> mag_field_vec;
    vector<double>   const_mag_field_vec;

    TGeoNode* world_{nullptr};

    std::vector<double> layer_width_tag;
    std::vector<double> layer_length_tag;
    std::vector<double> layer_thickness_tag;
    std::vector<int> strip_no_tag;
    std::vector<double> angles_tag;
    double center_x_tag_{0.};
    double center_y_tag_{0.};
    double center_z_tag_{0.};
    double length_x_tag_{0.};
    double length_y_tag_{0.};
    double length_z_tag_{0.};

    std::vector<double> layer_width_rec;
    std::vector<double> layer_length_rec;
    std::vector<double> layer_thickness_rec;
    std::vector<int> strip_no_rec;
    std::vector<double> angles_rec;
    double center_x_rec_{0.};
    double center_y_rec_{0.};
    double center_z_rec_{0.};
    double length_x_rec_{0.};
    double length_y_rec_{0.};
    double length_z_rec_{0.};

    double ECAL_center_x{-INFINITY};
    double ECAL_center_y{-INFINITY};
    double ECAL_center_z{-INFINITY};
    double ECAL_length_x{-INFINITY};
    double ECAL_length_y{-INFINITY};
    double ECAL_length_z{-INFINITY};
    std::vector<double> ECal_cell_length_x;
    std::vector<double> ECal_cell_length_y;
    std::vector<double> ECal_cell_length_z;
    int N_ECal_cell_x{0};
    int N_ECal_cell_y{0};
    int N_ECal_cell_z{0};
    int N_ECal_cells{0};
    double ECAL_cell_dx{0};
    double ECAL_cell_dy{0};
    double ECAL_cell_dz{0};
    std::array<int,3> N_ECal_block_per_region{0,0,0};
    std::array<int,3> N_ECal_cell_per_block{0,0,0};
    TVector3* ECAL_pos0{nullptr};
    std::vector<TVector3> ECAL_posmap{};
    std::vector<std::vector<int>> centerIdNeighborIds_staggered;
    std::vector<std::vector<int>> centerIdNeighborIds_legacy;

    //MCPHelperVec* helper{nullptr};
    DTruth *truth_{nullptr};

    std::unordered_map<std::string,int> processMap{};
    std::unordered_map<int,std::string> rev_processMap{};

private:
    AnaData();

};

extern AnaData *dAnaData;
#endif //DSIMU_ANADATA_H
