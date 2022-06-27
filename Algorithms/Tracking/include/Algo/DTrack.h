#ifndef DTRACK_H
#define DTRACK_H

#ifndef RETURN
#define RETURN std::nan("RETURN")
#endif

//................................................................................//
//CPP STL
#include <iostream>
#include <cmath>

//................................................................................//
//ROOT

//................................................................................//
//Framework
#include "Object/DMagnet.h"
#include "Object/ReconstructedParticle.h"

//................................................................................//
//Tracking
#include "Algo/TypeDef.h"

//................................................................................//
//Fitting methods implemented in Dark Shine tracking
//-- dNone: No method specified, return pre-fitting results from track finding
//-- dKalman: Kalman fitter from GenFit
enum FittingMethods {dNone, dKalman};

class DTrack
{
public:

    DTrack() = default;
    DTrack(const TrkHitPVec &newHits,
           double newPreR, 
           double newPreXc,
           double newPreYc,
           std::vector<double> magnets);
    DTrack(const DTrack &oldTrack);
    DTrack(DTrack &&oldTrack);

    ~DTrack() = default;

//................................................................................//
//Get
//................................................................................//
    int GetPDG()  const {return pdg;}
    int GetSign() const {return sign;}
    double GetPx() const {return px;}
    double GetPy() const {return py;}
    double GetPz() const {return pz;}
    double GetPp() const {return pp;}
    double GetPl() const {return py;}
    double GetECalSeedX() const {return ECal_seed_x;}
    double GetECalSeedY() const {return ECal_seed_y;}
    double GetECalDirctX() const {return ECal_seed_px;}
    double GetECalDirctY() const {return ECal_seed_py;}
    double GetECalQoP() const {return ECal_seed_pz;}

    int GetSize() const {return hits.size();}
    TrkHitP At(int i) {return hits.at(i);}
    double GetQuality() const {return quality;}

    double GetChi2() const {return chi2;}
    double GetXSigma() const {return xSigma;}
    double GetYSigma() const {return ySigma;}

//................................................................................//
//Set
//................................................................................//
    void SetPDG(int newPDG)  {pdg = newPDG;}
    void SetSign(int newSign) {sign = newSign;}
    void SetPx(double newPx) {px = newPx;}
    void SetPy(double newPy) {py = newPy;}
    void SetPz(double newPz) {pz = newPz;}
    void SetChi2(double newChi2) {chi2 = newChi2;}

//................................................................................//
//Processor
//................................................................................//
    void Fit(int method);
    void Evaluate();

private:
//................................................................................//
//Physical properties
    int pdg{11};
    int sign{-1};
    double px{RETURN};
    double py{RETURN};
    double pz{RETURN};
    double pp{RETURN};
    double pl{RETURN};
    double ECal_seed_x{RETURN};
    double ECal_seed_y{RETURN};
    double ECal_seed_px{RETURN};
    double ECal_seed_py{RETURN};
    double ECal_seed_pz{RETURN};

//................................................................................//
//Track properties
    double quality{RETURN};

//................................................................................//
//Detector properties
    double By{RETURN}; // manage problematic condition

//................................................................................//
//Fitting properties
    double chi2{RETURN};
    double xSigma{RETURN};
    double ySigma{RETURN};

//................................................................................//
//Prefitting properties
    double preR{RETURN};
    double preXc{RETURN};
    double preYc{RETURN};
 
//................................................................................//
//Hits collection
    TrkHitPVec hits;
};

#endif
