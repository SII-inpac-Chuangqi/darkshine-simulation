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
#include "Algo/Utils/Util.h"
#include "Algo/Object/TrkHit.h"
#include "Algo/Object/DTrack.h"
#include "Algo/LegacyFinder/GreedyFinder.h"

//................................................................................//
//public:
//................................................................................//
//Constructor
//
GreedyFinder::GreedyFinder(Config config, Pool *pool) : config_(config)
{
    circle_No_ = 0;

    GreedyLooping(pool);
    SortHits();
    //CutTracks();
}

void GreedyFinder::FillTracks(std::vector<std::shared_ptr<DTrack>> *tracks)
{
    tracks->clear();

    for(int i = 0; i < this->GetTrackNo(); i++)
    {
        tracks->push_back(std::make_shared<DTrack>(tracks_chosen_.at(i),
                                                   this->GetR(i),         //used in Kalman filter
                                                   this->GetCenterX(i),
                                                   this->GetCenterY(i)));
        for(int j = 0; j < tracks->at(i)->Size(); j++)
            tracks->at(i)->At(j)->SetTrack(tracks->at(i));
    }

    tracks_ = tracks;
}

std::vector<std::shared_ptr<DTrack>>* GreedyFinder::GetTracks()
{
    return tracks_;
}

//................................................................................//
//private:
//................................................................................//

//................................................................................//
//Finding method
//................................................................................//
//
TrkHitSPVecMap GreedyFinder::GetTempHitMap(Pool *pool)
{
     if(pool->IsNull()) return {{}};

     auto temp_pool_structured = **pool;
     if(temp_pool_structured.size() <= 2) return temp_pool_structured;

     int *layers = new int[temp_pool_structured.size() - 2];

     size_t i = 0;
     for(const auto &layer : temp_pool_structured)
     {
        if(layer.first != temp_pool_structured.begin() ->first &&
           layer.first != temp_pool_structured.rbegin()->first)
        {
            layers[i] = layer.first;
            i++;
        }
     }

     for(i = 0; i < temp_pool_structured.size() - 2; i++)
     {
         auto layer = temp_pool_structured.extract(layers[i]);
         layer.key() = -layer.key();
         temp_pool_structured.insert(std::move(layer));
     }

     delete[] layers;
     return temp_pool_structured;
}

//Finding control
void GreedyFinder::GreedyLooping(Pool *pool)
{
    TrkHitSPVecMap temp_pool_structured = GetTempHitMap(pool);

    for(;;)
    {
        auto it_map = temp_pool_structured.end();
        if(!GreedyLooping(temp_pool_structured, it_map, circle_No_)) return;
        //std::cout << goodness_Kasa_ << ", n hit " << hits_chosen_.size() << std::endl;

        if(goodness_Kasa_ > config_.goodness_cut && static_cast<int>(hits_chosen_.size()) > config_.min_depth)
        {
            tracks_chosen_.push_back(hits_chosen_);
            r_.push_back(r_Kasa_);
            center_x_.push_back(center_x_Kasa_);
            center_y_.push_back(center_y_Kasa_);
            goodness_.push_back(goodness_Kasa_);                                      
 
            auto it_erase_map = temp_pool_structured.end();
            for(size_t i = 0; i < hits_chosen_.size(); i++)
            {
                it_erase_map--;
                it_erase_map->second.erase(it_erase_map->second.begin() + hits_no_chosen_.at(i));
            }

            it_erase_map = temp_pool_structured.begin();
            while(it_erase_map != temp_pool_structured.end())
            {
                if(it_erase_map->second.size() == 0) temp_pool_structured.erase(it_erase_map++);
                else                                ++it_erase_map;
            }

            hits_chosen_.clear();
            hits_no_chosen_.clear();

            StoredClear();

            circle_No_ = 0;
            goodness_Kasa_ = -INFINITY;
        }
        else return;

        if(static_cast<int>(temp_pool_structured.size()) <= config_.min_depth) return;
    }

}

bool GreedyFinder::GreedyLooping(TrkHitSPVecMap &clustered_trk_hits_in_layer,
                                 TrkHitSPVecMap::iterator it_map,
                                 int cirNo)
{
    circle_No_++;
    //if(circle_No_%10000 == 0) std::cout << circle_No_ << std::endl;
    if(circle_No_ >= config_.max_circle) return false;

    it_map--;
    if(it_map == clustered_trk_hits_in_layer.begin())
    {
        for(size_t hits_no = 0; hits_no < it_map->second.size(); hits_no++)
        {
            StoredPushBack((*it_map->second.at(hits_no)).GetX(), (*it_map->second.at(hits_no)).GetZ(), (*it_map->second.at(hits_no)).GetY(),
                           hits_no, it_map->second.at(hits_no));

            double cur_A;
            double cur_B;
            double cur_R;
            double cur_goodness;
            MethodLooping(x_store_, y_store_, cur_A, cur_B, cur_R, cur_goodness);

            const size_t N = x_store_.size();
            bool calibrtion_cut = true;

            if(N > 2)
            {
                double *x = &y_store_.at(0);
                double *y = &oth_store_.at(0);
                double abr[3];
                tracking::LinearFit(abr, x, y, 2);

                for(size_t ii = 0; ii < N; ii++)
                {
                    double dis = tracking::PointToLineDistance(abr[0], -1., abr[1], x[ii], y[ii]);
                    //std::cout << "ii " << ii << " dis " << dis << std::endl;
                    calibrtion_cut = dis < config_.cut_y;
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

            StoredEraseEnd();                                                                    
        }
	
        return true;
    }

    for(size_t hits_no = 0; hits_no < it_map->second.size(); ++hits_no)
    {
        StoredPushBack((*it_map->second.at(hits_no)).GetX(), (*it_map->second.at(hits_no)).GetZ(), (*it_map->second.at(hits_no)).GetY(),
                       hits_no, it_map->second.at(hits_no));
        GreedyLooping(clustered_trk_hits_in_layer, it_map--, cirNo);

        StoredEraseEnd();
        it_map++; 
    }

    return true;
}

//
void GreedyFinder::CutTracks()
{
    std::vector<BadGuy> bad_guys;

    std::sort(bad_guys.begin(), bad_guys.end(), [](BadGuy bad_guy1, BadGuy bad_guy2)
                                                { return bad_guy1.how_bad < bad_guy2.how_bad; });
    for(const auto bad_guy : bad_guys) std::cout << bad_guy.how_bad << "\t";
    std::cout << std::endl;
}

void GreedyFinder::SortHits()
{
    for(auto &track : tracks_chosen_)
        std::sort(track.begin(), track.end(), [](const TrkHitSP &hit1, const TrkHitSP &hit2)
                                              { return hit1->GetCellIdZ() > hit2->GetCellIdZ(); } );
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
