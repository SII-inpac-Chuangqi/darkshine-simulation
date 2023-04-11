//................................................................................//
//ROOT
#include <vector>
#include <map>
#include <cmath>

//................................................................................//
//ROOT
#include "TMath.h"

//................................................................................//
//FRAMEWORK

//................................................................................//
//TRACKING
#include "Algo/TypeDef.h"
#include "Algo/Util.h"
#include "Algo/TrkHit.h"
#include "Algo/GreedyFinder.h"

#ifndef CUT_Y
#define CUT_Y 1.8
#endif

//................................................................................//
//public:
//................................................................................//
//Constructor
//
GreedyFinder::GreedyFinder(TrkHitPVecMap &clusteredTrkHitsInLayer, int newMinDepth, double newGoodnessCut)
{
    circleNo = 0;
    minDepth = newMinDepth;
    goodnessCut = newGoodnessCut;

    GreedyLooping(clusteredTrkHitsInLayer);
    //CutTracks();
    SortTracks();
}

//................................................................................//
//private:
//................................................................................//

//................................................................................//
//Finding method
//................................................................................//
//
void GreedyFinder::CutTracks()
{
}

void GreedyFinder::SortTracks()
{
    for(auto &track : tracks_chosen_)
        std::sort(track.begin(), track.end(), [](std::shared_ptr<TrkHit> &hit1, std::shared_ptr<TrkHit> &hit2)
                                              { return hit1->GetCellIdZ() > hit2->GetCellIdZ(); } );
}

//
TrkHitPVecMap GreedyFinder::GetTempHitMap(TrkHitPVecMap &clusteredTrkHitsInLayer)
{
     auto temp_ClusteredTrkHitsInLayer = clusteredTrkHitsInLayer;
     if(temp_ClusteredTrkHitsInLayer.size() <= 2) return temp_ClusteredTrkHitsInLayer;

     int *layers = new int[temp_ClusteredTrkHitsInLayer.size() - 2];

     size_t i = 0;
     for(const auto &layer : temp_ClusteredTrkHitsInLayer)
     {
        if(layer.first != temp_ClusteredTrkHitsInLayer.begin()->first &&
           layer.first != temp_ClusteredTrkHitsInLayer.end()  ->first)
        {
            layers[i] = layer.first;
            i++;
        }
     }

     for(i = 0; i < temp_ClusteredTrkHitsInLayer.size() - 2; i++)
     {
         auto layer = temp_ClusteredTrkHitsInLayer.extract(layers[i]);
         layer.key() = -layer.key();
         temp_ClusteredTrkHitsInLayer.insert(std::move(layer));
     }

     delete[] layers;
     return temp_ClusteredTrkHitsInLayer;
}

//Finding control
void GreedyFinder::GreedyLooping(TrkHitPVecMap &clusteredTrkHitsInLayer)
{
    TrkHitPVecMap temp_ClusteredTrkHitsInLayer = GetTempHitMap(clusteredTrkHitsInLayer);
    for(;;)
    {
        auto itMap = temp_ClusteredTrkHitsInLayer.end();
        GreedyLooping(temp_ClusteredTrkHitsInLayer, itMap, circleNo);
        //std::cout << goodness_Kasa_ << ", n hit " << hits_chosen_.size() << std::endl;

        //if(goodness[circleNo] > goodnessCut && static_cast<int>(hits_chosen_.size()) > minDepth)
        if(goodness_Kasa_ > goodnessCut && static_cast<int>(hits_chosen_.size()) > minDepth)
        {
            tracks_chosen_.push_back(hits_chosen_);
            r_.push_back(r_Kasa_);
            center_x_.push_back(center_x_Kasa_);
            center_y_.push_back(center_y_Kasa_);
            goodness_.push_back(goodness_Kasa_);                                      
 
            auto it_eraseMap = temp_ClusteredTrkHitsInLayer.end();
            for(size_t i = 0; i < hits_chosen_.size(); i++)
            {
                it_eraseMap--;
                it_eraseMap->second.erase(it_eraseMap->second.begin() + hits_no_chosen_.at(i));
            }

            it_eraseMap = temp_ClusteredTrkHitsInLayer.begin();
            while(it_eraseMap != temp_ClusteredTrkHitsInLayer.end())
            {
                if(it_eraseMap->second.size() == 0) temp_ClusteredTrkHitsInLayer.erase(it_eraseMap++);
                else                                ++it_eraseMap;
            }

            hits_chosen_.clear();
            hits_no_chosen_.clear();

            x_store_.clear();
            y_store_.clear();
            oth_store_.clear();
            hits_store_.clear();
            hits_no_store_.clear();

            circleNo = 0;
            goodness_Kasa_ = -INFINITY;
        }
        else break;

        //if(circleNo >= MAX_CIRCLE - 1) break;

        if(static_cast<int>(temp_ClusteredTrkHitsInLayer.size()) < minDepth) break;
    }

}

void GreedyFinder::GreedyLooping(TrkHitPVecMap &clusteredTrkHitsInLayer,
                                  TrkHitPVecMap::iterator itMap,
                                  int cirNo)
{
    circleNo++;
    //if(circleNo%10000 == 0) std::cout << circleNo << std::endl;
    if(circleNo >= MAX_CIRCLE) return;

    itMap--;
    if(itMap == clusteredTrkHitsInLayer.begin())
    {
        for(size_t hitsNo = 0; hitsNo < itMap->second.size(); hitsNo++)
        {
            x_store_.push_back((*itMap->second.at(hitsNo)).GetX());
            y_store_.push_back((*itMap->second.at(hitsNo)).GetZ());
            oth_store_.push_back((*itMap->second.at(hitsNo)).GetY());
            hits_store_.emplace_back(itMap->second.at(hitsNo));
            hits_no_store_.push_back(hitsNo);

            double cur_A;
            double cur_B;
            double cur_R;
            double cur_goodness;
            MethodLooping(x_store_, y_store_, cur_A, cur_B, cur_R, cur_goodness);
            //if(goodness[cirNo] < goodnessKasa)
            //{
            //    r[cirNo] = rKasa;
            //    centerX[cirNo] = centerXKasa;
            //    centerY[cirNo] = centerYKasa;
            //    goodness[cirNo] = goodnessKasa;

            //    hits_chosen_.assign(hits_store_.begin(), hits_store_.end());
            //    hits_no_chosen_.assign(hits_no_store_.begin(), hits_no_store_.end());
            //}

            const size_t N = x_store_.size();
            bool calibrtion_cut = true;

            if(N > 2)
            {
                double *x = &y_store_.at(0);
                double *y = &oth_store_.at(0);
                double abr[3];
                LinearFit(abr, x, y, 2);

                for(size_t ii = 0; ii < N; ii++)
                {
                    double dis = PointToLineDistance(abr[0], -1., abr[1], x[ii], y[ii]);
                    //std::cout << "ii " << ii << " dis " << dis << std::endl;
                    calibrtion_cut = dis < 6.;
                    if(!calibrtion_cut) break;
                }
            }
            //std::cout << std::endl;

            if(calibrtion_cut && cur_goodness > goodness_Kasa_)
            {
                //std::cout << "before: " << calibrtion_cut << "," << cur_goodness << std::endl;

                goodness_Kasa_ = cur_goodness;
                center_x_Kasa_ = cur_A;
                center_y_Kasa_ = cur_B;
                r_Kasa_        = cur_R;
                hits_chosen_.assign(hits_store_.begin(),     hits_store_.end());
                hits_no_chosen_.assign(hits_no_store_.begin(), hits_no_store_.end());

                //std::cout << "after: " << calibrtion_cut << "," << goodness_Kasa_ << std::endl;
            }
                                                                    
            x_store_.erase(x_store_.end() - 1);
            y_store_.erase(y_store_.end() - 1);
            oth_store_.erase(oth_store_.end() - 1);
            hits_store_.erase(hits_store_.end() - 1);
            hits_no_store_.erase(hits_no_store_.end() - 1);
        }
	
        return;
    }

    for(size_t hitsNo = 0; hitsNo < itMap->second.size(); ++hitsNo)
    {
        hits_no_store_.push_back(hitsNo);
        hits_store_.emplace_back(itMap->second.at(hitsNo));
        x_store_.push_back((*itMap->second.at(hitsNo)).GetX());
	y_store_.push_back((*itMap->second.at(hitsNo)).GetZ());
        oth_store_.push_back((*itMap->second.at(hitsNo)).GetY());

        GreedyLooping(clusteredTrkHitsInLayer, itMap--, cirNo);

        hits_no_store_.erase(hits_no_store_.end() - 1);
        hits_store_.erase(hits_store_.end() - 1);
        x_store_.erase(x_store_.end() - 1);
        y_store_.erase(y_store_.end() - 1);
        oth_store_.erase(oth_store_.end() - 1);
        itMap++; 
    }

    return;
}

//................................................................................//
//Kasa method

void GreedyFinder::MethodLooping(const std::vector<double> &track_x, const std::vector<double> &track_y,
                                  double &cur_A, double &cur_B, double &cur_R, double &cur_goodness)
{
    MethodKasa(track_x, track_y, cur_A, cur_B, cur_R, cur_goodness);
}

double GreedyFinder::MethodKasa(const std::vector<double> &track_x, const std::vector<double> &track_y,
                                 double &cur_A, double &cur_B, double &cur_R, double &cur_goodness)
{
    if(track_x.size() != track_y.size())
    {
	std::cout << "[ERROR] ==> x and y have different sizes" << std::endl;
        return 0.;
    }

    const int pointNo = track_x.size();

    double X1 = 0.;
    double Y1 = 0.;
    double X2 = 0.;
    double Y2 = 0.;
    double X3 = 0.;
    double Y3 = 0.;
    double X1Y1 = 0.;
    double X1Y2 = 0.;
    double X2Y1 = 0.;

    for(int i = 0; i < pointNo; ++i)
    {
        X1 += track_x[i];
        Y1 += track_y[i];
        X2 += track_x[i]*track_x[i];
        Y2 += track_y[i]*track_y[i];
        X3 += track_x[i]*track_x[i]*track_x[i];
        Y3 += track_y[i]*track_y[i]*track_y[i];
        X1Y1 += track_x[i]*track_y[i];
        X1Y2 += track_x[i]*track_y[i]*track_y[i];
        X2Y1 += track_x[i]*track_x[i]*track_y[i];
    }

    double C, D, E, G, H, N;
    double a, b, c;
    N = pointNo;
    C = N*X2 - X1*X1;
    D = N*X1Y1 - X1*Y1;
    E = N*X3 + N*X1Y2 - (X2 + Y2)*X1;
    G = N*Y2 - Y1*Y1;
    H = N*X2Y1 + N*Y3 - (X2 + Y2)*Y1;
    a = (H*D - E*G)/(C*G - D*D);
    b = (H*C - E*D)/(D*D - G*C);
    c = -(a*X1 + b*Y1 + X2 + Y2)/N;

    double A, B, R;
    A = -0.5*a;
    B = -0.5*b;
    R = 0.5*sqrt(a*a + b*b - 4*c);

    //centerXKasa = A;
    //centerYKasa = B;
    //rKasa = R;
    cur_A = A;
    cur_B = B;
    cur_R = R;

    double s = 0.;
    for (int i = 0; i < pointNo; i++)
    {
        double x = track_x.at(i) - A;
        double y = track_y.at(i) - B;
        double z = sqrt(x*x + y*y);
        s += (R - z)*(R - z);
    }
    //goodnessKasa = 1 - sqrt(s/(pointNo*R*R));
    cur_goodness = 1 - sqrt(s/(pointNo*R*R));

    return 0.;
}
