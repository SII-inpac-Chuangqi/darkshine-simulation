//................................................................................//
//CPP STL
#include <iostream>

//................................................................................//
//ROOT

//................................................................................//
//Framework

//................................................................................//
//Tracking
#include "Algo/DTrack.h"
#include "Algo/KalmanFitting.h"

DTrack::DTrack(const TrkHitPVec &newHits,
               double newPreR, double newPreXc, double newPreYc, 
               double RecTrk_B)                                  : By(RecTrk_B),
                                                                   preR(newPreR),
                                                                   preXc(newPreXc),
                                                                   preYc(newPreYc),
                                                                   hits(newHits)
{}

DTrack::DTrack(const DTrack &oldTrack) : pdg(oldTrack.pdg),
                                         sign(oldTrack.sign),
                                         px(oldTrack.px),
                                         py(oldTrack.py),
                                         pz(oldTrack.pz),
                                         pp(oldTrack.pp),
                                         pl(oldTrack.pl),
                                         By(oldTrack.By),
                                         chi2(oldTrack.chi2),
                                         xSigma(oldTrack.xSigma),
                                         ySigma(oldTrack.ySigma),
                                         preR(oldTrack.preR),
                                         preXc(oldTrack.preXc),
                                         preYc(oldTrack.preYc),
                                         hits(oldTrack.hits)
{}

DTrack::DTrack(DTrack &&oldTrack) : pdg(std::move(oldTrack.pdg)),
                                    sign(std::move(oldTrack.sign)),
                                    px(std::move(oldTrack.px)),
                                    py(std::move(oldTrack.py)),
                                    pz(std::move(oldTrack.pz)),
                                    pp(std::move(oldTrack.pp)),
                                    pl(std::move(oldTrack.pl)),
                                    By(std::move(oldTrack.By)),
                                    chi2(std::move(oldTrack.chi2)),
                                    xSigma(std::move(oldTrack.xSigma)),
                                    ySigma(std::move(oldTrack.ySigma)),
                                    preR(std::move(oldTrack.preR)),
                                    preXc(std::move(oldTrack.preXc)),
                                    preYc(std::move(oldTrack.preYc)),
                                    hits(oldTrack.hits)
{
    oldTrack.hits.clear();
}

void DTrack::Fit(int method)
{
    Fitting *fitter = nullptr;

    switch(method)
    {
        case dKalman :
                       fitter = new KalmanFitting(hits, preR, By);
                       break;
    }

    px = fitter->GetPx();
    py = fitter->GetPy();
    pz = fitter->GetPz();
    pp = fitter->GetPp();
    pl = fitter->GetPl();
    chi2 = fitter->GetChi2();
    xSigma = fitter->GetXSigma();
    ySigma = fitter->GetYSigma();

    if(!fitter)
        delete fitter;
    fitter = nullptr;
}
