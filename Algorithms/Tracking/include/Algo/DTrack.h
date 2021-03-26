#ifndef DTRACK_H
#define DTRACK_H

#ifndef RETURN
#define RETURN INFINITY
#endif

//................................................................................//
//CPP STL
#include <iostream>
#include <cmath>

//................................................................................//
//ROOT

//................................................................................//
//Framework
#include "Object/ReconstructedParticle.h"

//................................................................................//
//Tracking
#include "Algo/TypeDef.h"

enum FittingMethods {dKalman};

class DTrack
{
public:

    DTrack() = default;
    DTrack(const TrkHitPVec &newHits,
           double newPreR, double newPreXc, double newPreYc,
           double By);
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
    double GetPp() const {return sqrt(px*px + pz*pz);}
    double GetPl() const {return py;}

    int GetSize() const {return hits.size();}
    TrkHitP At(int i) {return hits.at(i);}

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

private:
//................................................................................//
//Physical properties
    int pdg = 11;
    int sign = -1;
    double px = INFINITY;
    double py = INFINITY;
    double pz = INFINITY;
    double pp = INFINITY;
    double pl = INFINITY;

//................................................................................//
//Track properties

//................................................................................//
//Detector properties, better to read from framework
    double By = INFINITY;

//................................................................................//
//Fitting properties
    double chi2 = INFINITY;
    double xSigma = INFINITY;
    double ySigma = INFINITY;

//................................................................................//
//Prefitting properties
    double preR = INFINITY;
    double preXc = INFINITY;
    double preYc = INFINITY;
 
//................................................................................//
//Hits collection
    TrkHitPVec hits;
};

#endif
