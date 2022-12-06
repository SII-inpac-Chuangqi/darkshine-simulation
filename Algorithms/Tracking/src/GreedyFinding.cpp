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
#include "Algo/TrkHit.h"
#include "Algo/GreedyFinding.h"

//................................................................................//
//public:
//................................................................................//
//Constructor
//
GreedyFinding::GreedyFinding(TrkHitPVecMap &clusteredTrkHitsInLayer, int newMinDepth, double newGoodnessCut)
{
    minDepth = newMinDepth;
    goodnessCut = newGoodnessCut;
    GreedyLooping(clusteredTrkHitsInLayer);
}

//................................................................................//
//private:
//................................................................................//

//................................................................................//
//Fitting method
//................................................................................//
//Fitting control
void GreedyFinding::GreedyLooping(TrkHitPVecMap &clusteredTrkHitsInLayer)
{
    TrkHitPVecMap tempClusteredTrkHitsInLayer = clusteredTrkHitsInLayer;
    for(;;)
    {
        auto itMap = tempClusteredTrkHitsInLayer.end();
        GreedyLooping(tempClusteredTrkHitsInLayer, itMap, circleNo);
        if(goodness[circleNo] > goodnessCut && static_cast<int>(hitChosen.size()) > minDepth)
        {
            VecHitChosen.emplace_back(hitChosen);

            auto it_eraseMap = tempClusteredTrkHitsInLayer.end();
            for(size_t i = 0; i < hitChosen.size(); i++)
            {
                it_eraseMap--;
                it_eraseMap->second.erase(it_eraseMap->second.begin() + hitNoChosen.at(i));
            }

            it_eraseMap = tempClusteredTrkHitsInLayer.begin();
            while(it_eraseMap != tempClusteredTrkHitsInLayer.end())
            {
                if(it_eraseMap->second.size() == 0) tempClusteredTrkHitsInLayer.erase(it_eraseMap++);
                else                                ++it_eraseMap;
            }

            hitChosen.clear();
            hitNoChosen.clear();

            xStore.clear();
            yStore.clear();
            hitStore.clear();
            hitNoStore.clear();

            circleNo++;
        }
        else break;

        if(static_cast<int>(tempClusteredTrkHitsInLayer.size()) < minDepth) break;
    }

}

void GreedyFinding::GreedyLooping(TrkHitPVecMap &clusteredTrkHitsInLayer,
                                  TrkHitPVecMap::iterator itMap,
                                  int cirNo)
{
    itMap--;
    if(itMap == clusteredTrkHitsInLayer.begin())
    {
        for(size_t hitsNo = 0; hitsNo < itMap->second.size(); hitsNo++)
        {
            xStore.push_back((*itMap->second.at(hitsNo)).GetU());
            yStore.push_back((*itMap->second.at(hitsNo)).GetZ());
            hitStore.emplace_back(itMap->second.at(hitsNo));
            hitNoStore.push_back(hitsNo);

            MethodLooping(xStore, yStore);
            if(goodness[cirNo] < goodnessKasa)
            {
                r[cirNo] = rKasa;
                centerX[cirNo] = centerXKasa;
                centerY[cirNo] = centerYKasa;
                goodness[cirNo] = goodnessKasa;

                hitChosen.assign(hitStore.begin(), hitStore.end());
                hitNoChosen.assign(hitNoStore.begin(), hitNoStore.end());
            }

            xStore.erase(xStore.end() - 1);
            yStore.erase(yStore.end() - 1);
            hitStore.erase(hitStore.end() - 1);
            hitNoStore.erase(hitNoStore.end() - 1);
        }
	
        return;
    }

    for(size_t hitsNo = 0; hitsNo < itMap->second.size(); ++hitsNo)
    {
        hitNoStore.push_back(hitsNo);
        hitStore.emplace_back(itMap->second.at(hitsNo));
        xStore.push_back((*itMap->second.at(hitsNo)).GetU());
	yStore.push_back((*itMap->second.at(hitsNo)).GetZ());

        GreedyLooping(clusteredTrkHitsInLayer, itMap--, cirNo);

        hitNoStore.erase(hitNoStore.end() - 1);
        hitStore.erase(hitStore.end() - 1);
        xStore.erase(xStore.end() - 1);
        yStore.erase(yStore.end() - 1);
        itMap++; 
    }

    return;
}

//................................................................................//
//Kasa method

void GreedyFinding::MethodLooping(const std::vector<double> &track_x, const std::vector<double> &track_y)
{
    double A(0.), B(0.);

    MethodKasa(track_x, track_y, A, B);
}

double GreedyFinding::MethodKasa(const std::vector<double> &track_x, const std::vector<double> &track_y,
                                 [[maybe_unused]] const double &cur_A, [[maybe_unused]] const double &cur_B)
{
    if(track_x.size() != track_y.size())
    {
	std::cout << "x and y have different sizes" << std::endl;
        return 0.;
    }

    int pointNo = track_x.size();

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

    centerXKasa = A;
    centerYKasa = B;
    rKasa = R;

    double s = 0.;
    for (int i = 0; i < pointNo; i++)
    {
        double x = track_x.at(i) - A;
        double y = track_y.at(i) - B;
        double z = sqrt(x*x + y*y);
        s += (R - z)*(R - z);
    }
    goodnessKasa = 1 - sqrt(s/(pointNo*R*R));

    return 0.;
}
