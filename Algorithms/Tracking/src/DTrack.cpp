//................................................................................//
//CPP STL
#include <iostream>
#include <map>
#include <vector>

//................................................................................//
//ROOT

//................................................................................//
//Framework

//................................................................................//
//Tracking
#include "Algo/DTrack.h"
#include "Algo/KalmanFitting.h"

//................................................................................//
//public:
//................................................................................//
//Constructor
//
DTrack::DTrack(const TrkHitPVec &newHits,
               double newPreR,
               double newPreXc,
               double newPreYc, 
               std::vector<double> magnets) : By(magnets.at(1)),
                                              preR(newPreR),
                                              preXc(newPreXc),
                                              preYc(newPreYc),
                                              hits(newHits)
{
}

DTrack::DTrack(const DTrack &oldTrack) : pdg(oldTrack.pdg),         //physical properties
                                         sign(oldTrack.sign),
                                         px(oldTrack.px),
                                         py(oldTrack.py),
                                         pz(oldTrack.pz),
                                         pp(oldTrack.pp),
                                         pl(oldTrack.pl),
                                         ECal_seed_x(oldTrack.ECal_seed_x),
                                         ECal_seed_y(oldTrack.ECal_seed_y),
                                         ECal_seed_px(oldTrack.ECal_seed_px),
                                         ECal_seed_py(oldTrack.ECal_seed_py),
                                         ECal_seed_pz(oldTrack.ECal_seed_pz),

                                         quality(oldTrack.quality), //track properties

                                         By(oldTrack.By),           //detector properties

                                         chi2(oldTrack.chi2),       //fitting properties
                                         xSigma(oldTrack.xSigma),
                                         ySigma(oldTrack.ySigma),

                                         preR(oldTrack.preR),       //prefitting properties
                                         preXc(oldTrack.preXc),
                                         preYc(oldTrack.preYc),

                                         hits(oldTrack.hits)        //hits collection
{}

DTrack::DTrack(DTrack &&oldTrack) : pdg(std::move(oldTrack.pdg)),         //physical properties
                                    sign(std::move(oldTrack.sign)),
                                    px(std::move(oldTrack.px)),
                                    py(std::move(oldTrack.py)),
                                    pz(std::move(oldTrack.pz)),
                                    pp(std::move(oldTrack.pp)),
                                    pl(std::move(oldTrack.pl)),
                                    ECal_seed_x(std::move(oldTrack.ECal_seed_x)),
                                    ECal_seed_y(std::move(oldTrack.ECal_seed_y)),
                                    ECal_seed_px(std::move(oldTrack.ECal_seed_px)),
                                    ECal_seed_py(std::move(oldTrack.ECal_seed_py)),
                                    ECal_seed_pz(std::move(oldTrack.ECal_seed_pz)),

                                    quality(std::move(oldTrack.quality)), //track properties

                                    By(std::move(oldTrack.By)),           //detector properties

                                    chi2(std::move(oldTrack.chi2)),       //fitting properties
                                    xSigma(std::move(oldTrack.xSigma)),
                                    ySigma(std::move(oldTrack.ySigma)),

                                    preR(std::move(oldTrack.preR)),       //prefitting properties
                                    preXc(std::move(oldTrack.preXc)),
                                    preYc(std::move(oldTrack.preYc)),

                                    hits(oldTrack.hits)                   //hits collection
{
    oldTrack.hits.clear();
}

DTrack& DTrack::operator=(const DTrack &old_track)
{
    if(this == &old_track) return *this;

    verbose_ = old_track.verbose_;

    pdg = old_track.pdg;
    sign = old_track.sign;
    px = old_track.px;
    py = old_track.py;
    pz = old_track.pz;
    pp = old_track.pp;
    pl = old_track.pl;
    ECal_seed_x = old_track.ECal_seed_x;
    ECal_seed_y = old_track.ECal_seed_y;
    ECal_seed_px = old_track.ECal_seed_px;
    ECal_seed_py = old_track.ECal_seed_py;
    ECal_seed_pz = old_track.ECal_seed_pz;

    quality = old_track.quality;

    By = old_track.By; 

    chi2 = old_track.chi2;
    xSigma = old_track.xSigma;
    ySigma = old_track.ySigma;

    preR = old_track.preR;
    preXc = old_track.preXc;
    preYc = old_track.preYc;

    hits.clear();
    hits.assign(old_track.hits.begin(), old_track.hits.end());
}

void DTrack::Fit(int method)
{
    Fitting *fitter = nullptr;

    switch(method)
    {
        case dKalman  :
                        fitter = new KalmanFitting(hits,
                                                   {preR,   //Fix to 2 ordered parameters! --bending radius as fitting seed
                                                    By},    //                             --magnet value to manage exception condition
                                                   verbose_ //Verbose
                                                  );
                        break;
        case dNone    :
                        if(verbose_ > 0)
                            std::cout << "[INFO] ==> Fit not required" << std::endl;
                        break;
        default :
                        if(verbose_ > 0)
                            std::cerr << "[WARNING] ==> Fit method not found. Use default fitter GenFit Kalman fitter" << std::endl;
                        fitter = new KalmanFitting(hits,
                                                   {preR,   //Fix to 2 ordered parameters! --bending radius as fitting seed
                                                    By},    //                             --magnet value to manage exception condition
                                                   verbose_ //Verbose
                                                  );
    }

    if(fitter)
    {
        px = fitter->GetPx();
        py = fitter->GetPy();
        pz = fitter->GetPz();
        pp = fitter->GetPp();
        chi2 = fitter->GetChi2();
        xSigma = fitter->GetXSigma();
        ySigma = fitter->GetYSigma();
        ECal_seed_x = fitter->GetECalSeedX();
        ECal_seed_y = fitter->GetECalSeedY();
        ECal_seed_px = fitter->GetECalDirctX();
        ECal_seed_py = fitter->GetECalDirctY();
        ECal_seed_pz = fitter->GetECalQoP();
        //std::cout << ECal_seed_pz << std::endl;
    }
    else
        pp = 0.3*preR*std::abs(By);

    delete fitter;
    fitter = nullptr;
}

void DTrack::Evaluate()
{
    if(hits.size() <= 0)
    {
        quality = 0;
        return;
    }

    std::map<int, int> particleCount;
    for(auto hit : hits)
    {
        auto contribution = hit->getPContribution();
        for(auto particle : contribution)
        {
            auto itSearchParticle = particleCount.find(particle.getId());
            if(itSearchParticle != particleCount.end())
                itSearchParticle->second++;
            else
                particleCount.insert(std::pair(particle.getId(), 1));
        }
    }

    for(auto count : particleCount)
    {
        if(1.*count.second/hits.size() > quality)
            quality = 1.*count.second/hits.size();
    }
}
