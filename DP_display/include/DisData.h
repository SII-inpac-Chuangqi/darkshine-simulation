#ifndef DISDATA_H
#define DISDATA_H

#include <array>

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

private:
    DisData();

    TFile *geo_file_{nullptr};

    bool if_uniform_mag_{true};
    std::array<DMagnet*, 3> magnets_ = {nullptr, nullptr, nullptr};
};

extern DisData *dDisData;
#endif //DISDATA_H
