#include "DisData.h"

DisData *dDisData = nullptr;

DisData *DisData::CreateInstance()
{
    if (dDisData == nullptr)
        dDisData = new DisData();
    return dDisData;
}

DisData::DisData()
{
}

void DisData::SetMagnets()
{
    if(!geo_file_)
    {
        std::cerr << "[WARNING] ==> Geometry not loaded" << std::endl;
        if_uniform_mag_ = true;
        return;
    }

    if(!geo_file_->Get<DMagnet>("magnet0") ||
       !geo_file_->Get<DMagnet>("magnet1") ||
       !geo_file_->Get<DMagnet>("magnet2"))
    {
        std::cerr << "[WARNING] ==> Geometry not loaded" << std::endl;
        if_uniform_mag_ = true;
        return;
    }

    magnets_.at(0) = geo_file_->Get<DMagnet>("magnet0");
    magnets_.at(1) = geo_file_->Get<DMagnet>("magnet1");
    magnets_.at(2) = geo_file_->Get<DMagnet>("magnet2");
}

double DisData::GetMagnetXAt(double x, double y, double z)
{
    if(if_uniform_mag_) return 0.;

    return magnets_.at(0)->GetField(x, y, z);
}

double DisData::GetMagnetYAt(double x, double y, double z)
{
    if(if_uniform_mag_) return -1.5;

    return magnets_.at(1)->GetField(x, y, z);
}

double DisData::GetMagnetZAt(double x, double y, double z)
{
    if(if_uniform_mag_) return 0.;

    return magnets_.at(2)->GetField(x, y, z);
}
