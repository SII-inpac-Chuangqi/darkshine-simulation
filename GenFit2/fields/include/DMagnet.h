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
        int id; //ID of slice

        int gNVariables;    //Dimension of coordination
        int gNCoefficients; //No. of parameterization coefficients
        double gChi2;       //Chi2 of parameteriztion

        double gDMean;      //Mean value of magnet
        double gDMin;       //Min value of magnet
        double gDMax;       //Max calue of magnet

        std::vector<double> gXMin = std::vector<double>(3); //Min value of coordination
        std::vector<double> gXMax = std::vector<double>(3); //Max value of coordination
        std::vector<double> gCoefficients;                  //Coefficients of parameterization functions
        std::vector<int> gPowers;                           //Powers of parameterization
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
//Copy constructor
    DMagnet(const DMagnet &old);

//................................................................................//
//Operator =
    DMagnet& operator =(const DMagnet &rhs);

//................................................................................//
//Destructor
    ~DMagnet() {}

//................................................................................//

//................................................................................//
//No move constructor
    DMagnet(DMagnet &&magnet) = delete;

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

//................................................................................//
//Print
    virtual void Print(Option_t *option = "") const;

private:

    std::vector<MagSlice> mags_;    //vector of MagSlice to store parameterized fields actually
    std::vector<double> xDivision_; //vectors to store division of parameterized region: x nodes
    std::vector<double> yDivision_; //                                                   y nodes
    std::vector<double> zDivision_; //                                                   z nodes

//................................................................................//
//Get field value in chosen MagSlice
    double GetSliceField(double x, double y, double z, const MagSlice &mag);
};

#endif
