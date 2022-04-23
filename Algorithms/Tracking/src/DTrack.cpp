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

void DTrack::Fit(int method)
{
    Fitting *fitter = nullptr;

    switch(method)
    {
        case dKalman  :
                        fitter = new KalmanFitting(hits,
                                                   {preR, //Fix to 2 ordered parameters! --bending radius as fitting seed
                                                    By}   //                             --magnet value to manage exception condition
                                                  );
                        break;
        case dNone    :
                        std::cout << "No fitting." << std::endl;
                        break;
        default :
                        std::cout << "No fitter specified." << std::endl
                                  << "Use default fitter: " << std::endl;
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
        ECal_dirct_x = fitter->GetECalDirctX();
        ECal_dirct_y = fitter->GetECalDirctY();
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
