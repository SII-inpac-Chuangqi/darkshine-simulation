#ifndef FINDER_H
#define FINDER_H

//................................................................................//
//C++
#include <iostream>
#include <map>
#include <vector>
#include <memory>

//................................................................................//
//ROOT

//................................................................................//
//Framework

//................................................................................//
//Tracking
#include "Algo/TypeDef.h"
#include "Algo/TrkHit.h"
#include "Algo/DTrack.h"

class Finder
{
public:
//................................................................................//
//Constructor
    Finder() {}
    virtual ~Finder() {tracks_ = nullptr;}

    Finder(const Finder&) = delete;
    Finder& operator =(const Finder&) = delete;

//................................................................................//
//Get
    virtual void FillTracks(std::vector<std::shared_ptr<DTrack>> *tracks) = 0;

    virtual double GetR      (int i) const = 0;
    virtual double GetCenterX(int i) const = 0;
    virtual double GetCenterY(int i) const = 0;
    virtual double GetChi2   (int i) const = 0;
    virtual int    GetTrackNo(     ) const = 0;

    virtual std::vector<double> GetMagnetFieldAt(double x, double y, double z);

    virtual std::vector<TrkHitSPVec>::iterator First() = 0;
    virtual std::vector<TrkHitSPVec>::iterator Last () = 0;

protected:
    std::vector<std::shared_ptr<DTrack>> *tracks_;
};

#endif
