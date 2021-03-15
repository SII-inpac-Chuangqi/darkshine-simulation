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
GreedyFinding::GreedyFinding(TrkHitPVecMap &clusteredTrkHitsInLayer)
{
    GreedyLooping(clusteredTrkHitsInLayer);
}

//................................................................................//
//private:
//................................................................................//
//Calculate deflection
//................................................................................//
void GreedyFinding::Theta(int cirNo)
{
    if(hitChosen.size() < 1) return;
    if(r[cirNo] <= 0)        return;

    double a1, a2, b1, b2;
    a1 = (*hitChosen.at(0)).GetU() - centerX[cirNo];
    a2 = (*hitChosen.at(0)).GetZ() - centerY[cirNo];
    b1 = (*hitChosen.at(hitChosen.size() - 1)).GetU() - centerX[cirNo];
    b2 = (*hitChosen.at(hitChosen.size() - 1)).GetZ() - centerY[cirNo];

    double phi;
    phi = acos((a1*b1 + a2*b2)/r[cirNo]/r[cirNo]);
    theta[cirNo] = atan(phi*r[cirNo]/((*hitChosen.at(0)).GetV() - (*hitChosen.at(hitChosen.size() - 1)).GetV()));
}

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
        if(goodness[circleNo] > 0.99 && hitChosen.size() > 3)
        {
            VecHitChosen.push_back(hitChosen);
            Theta(circleNo);

            auto it_eraseMap = tempClusteredTrkHitsInLayer.end();
            for(int i = 0; i < static_cast<int>(hitChosen.size()); i++)
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

        if(tempClusteredTrkHitsInLayer.size() < 3) break;
    }

}

void GreedyFinding::GreedyLooping(TrkHitPVecMap &clusteredTrkHitsInLayer,
                            TrkHitPVecMap::iterator itMap,
                            int cirNo)
{
    itMap--;
    if(itMap == clusteredTrkHitsInLayer.begin())
    {
        for(int hitsNo = 0; hitsNo < static_cast<int>(itMap->second.size()); hitsNo++)
        {
            xStore.push_back((*itMap->second.at(hitsNo)).GetU());
            yStore.push_back((*itMap->second.at(hitsNo)).GetZ());
            hitStore.emplace_back(itMap->second.at(hitsNo));
            hitNoStore.push_back(hitsNo);

            MethodKasa(xStore, yStore);
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

    for(int hitsNo = 0; hitsNo < static_cast<int>(itMap->second.size()); ++hitsNo)
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
void GreedyFinding::MethodKasa(std::vector<double> xVec, std::vector<double> yVec)
{
    if(xVec.size() != yVec.size())
    {
	std::cout << "x and y have different sizes" << std::endl;
        return;
    }

    int pointNo = xVec.size();

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
        X1 += xVec[i];
        Y1 += yVec[i];
        X2 += xVec[i]*xVec[i];
        Y2 += yVec[i]*yVec[i];
        X3 += xVec[i]*xVec[i]*xVec[i];
        Y3 += yVec[i]*yVec[i]*yVec[i];
        X1Y1 += xVec[i]*yVec[i];
        X1Y2 += xVec[i]*yVec[i]*yVec[i];
        X2Y1 += xVec[i]*xVec[i]*yVec[i];
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
        double x = xVec.at(i) - A;
        double y = yVec.at(i) - B;
        double z = sqrt(x*x + y*y);
        s += (R - z)*(R - z);
    }
    goodnessKasa = 1 - sqrt(s/(pointNo*R*R));
}
