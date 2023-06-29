#ifndef DISDATA_H
#define DISDATA_H

#include <array>
#ifdef RM_UNIT
#define CUNIT 1
#else
#define CUNIT 10
#endif
//logic of length unit
// GEANT4 aleays use mm
// TGeo and TEve depends on version, and defined using CUNIT
// Plain number in DSimu/DAna/DDis always using mm

#include "TFile.h"

#include "Object/DMagnet.h"
#include "DEventReader_dis.h"

class DisData
{
public:
    DisData(const DisData &) = delete;

    DisData &operator=(DisData const &) = delete;

    static DisData *CreateInstance();

    void SetGeoFile(TString geo_name)
    {
        if(geo_file_) geo_file_->Close();

        geo_file_ = new TFile(geo_name, "read");
        SetMagnets();
    }
    TFile* GetGeoFile() const {return geo_file_;}

    void SetMagnets();

    double GetMagnetXAt(double x, double y, double z);
    double GetMagnetYAt(double x, double y, double z);
    double GetMagnetZAt(double x, double y, double z);

    bool isMagnets(){return !if_uniform_mag_;};

    void setBfieldRegionZleft(double z){
        std::cout<<"[DisData] B field region Z left(mm) "<<z<<std::endl;
        _B_field_region[0]=z;
    };
    void setBfieldRegionZright(double z){
        std::cout<<"[DisData] B field region Z right(mm) "<<z<<std::endl;
        _B_field_region[1]=z;
    };
    bool isInBfieldRegion([[maybe_unused]] double x,[[maybe_unused]] double y,double z){
        return z>=_B_field_region[0] && z<_B_field_region[1];
    };

private:
    DisData();

    TFile *geo_file_{nullptr};

    bool if_uniform_mag_{true}; // no magnets and using uniform instead
    std::array<DMagnet*, 3> magnets_ = {nullptr, nullptr, nullptr};

    std::array<double,2> _B_field_region{0,0};
};

extern DisData *dDisData;
#endif //DISDATA_H
