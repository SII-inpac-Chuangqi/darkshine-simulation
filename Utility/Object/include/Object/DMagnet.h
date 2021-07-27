#ifndef D_MAGNET_H
#define D_MAGNET_H

#include <math.h>
#include <vector>

#include "TNamed.h"
#include "TMultiDimFit.h"

class DMagnet : public TNamed
{
    ClassDef(DMagnet, 1);

//................................................................................//
//Struct to store parameterization from TMultiDimFit
    struct MagSlice
    {
        int id;

        int gNVariables;
        int gNCoefficients;
        double gDMean;

        std::vector<double> gXMin;
        std::vector<double> gXMax;
        std::vector<double> gCoefficients;
        std::vector<int> gPowers;
    };

public:
//................................................................................//
//Constructor & destructor
//................................................................................//
//Empty constructor, do not use
    DMagnet() {}
//................................................................................//
//Constructor with name & title needed
    DMagnet(const TString &name, const TString &title) {this->SetNameTitle(name, title);}
//................................................................................//
//Destructor
    ~DMagnet() {}

//................................................................................//
//No copy/move constructor & =
    DMagnet(const DMagnet &magnet) = delete;
    DMagnet(DMagnet &&magnet) = delete;
    DMagnet& operator =(const DMagnet &magnet) = delete;

//................................................................................//
//Set
//................................................................................//
//Set divisions of the parameterized region
    void SetXDivision(const std::vector<double> &division);
    void SetYDivision(const std::vector<double> &division);
    void SetZDivision(const std::vector<double> &division);
//................................................................................//
//Add parameterized magnet from TMultiDimFit
    void AddMagnet(const TMultiDimFit *fitter);

//................................................................................//
//Get
//................................................................................//
//Get divisions of the parameterized region
    std::vector<double> GetXDivision() const {return xDivision_;}
    std::vector<double> GetYDivision() const {return yDivision_;}
    std::vector<double> GetZDivision() const {return zDivision_;}
//................................................................................//
//Get MagSlice
    std::vector<MagSlice> GetMagnets() const {return mags_;}

//................................................................................//
//Get field value at position (x, y, z) in mm
    double GetField(double x, double y, double z);

private:

    std::vector<MagSlice> mags_;    //vector of MagSlice to store parameterized fields actually
    std::vector<double> xDivision_; //vectors to store division of parameterized region: x nodes
    std::vector<double> yDivision_; //                                                   y nodes
    std::vector<double> zDivision_; //                                                   z nodes

//................................................................................//
//Get field value in chosen MagSlice
    double GetFieldValue(double x, double y, double z, const MagSlice &mag);
};

#endif
