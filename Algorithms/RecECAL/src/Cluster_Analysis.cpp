//
// Created by Zhang Yulei on 12/27/20.
// Clustering Algo from Q.LIU from y2022.
//

#include <utility>
#include <numeric>
#include <functional>

#include "Algo/Cluster_Analysis.h"

Cluster_Analysis::Cluster_Analysis(CalorimeterHitVec *clusterVec, bool isStaggered) : ClusterVec(clusterVec) {
    std::sort(ClusterVec->begin(), ClusterVec->end(), sortbyE<CalorimeterHit>);
    E_Tot = FindETotal();
    setNX(dAnaData->getNECalCellX());
    setNY(dAnaData->getNECalCellY());
    setNZ(dAnaData->getNECalCellZ());
    
    m_isStaggered=isStaggered;       
}

CalorimeterHit *Cluster_Analysis::FindMaxEHit() {
    // return the hit with the maximal energy
    return ClusterVec->at(0);
}

double Cluster_Analysis::FindETotal() {
    // return the total deposit Energy
    double E = 0.;

    // Lambda Expression
    //for_each(ClusterVec->begin(), ClusterVec->end(), [&E] (CalorimeterHit* h) {E += h->getE();} );

    for (auto hit: *ClusterVec) E += hit->getE();

    return E;
}

double Cluster_Analysis::FindEFraction(unsigned n) {
    if (static_cast<int>(n) < 0) return -1;
    if (n >= ClusterVec->size()) n = ClusterVec->size();
    double E = 0.;

    double E_tmp_total = 0;
    for (auto hit: *ClusterVec) {
        if (hit->getE() < 0) cout << hit->getE() << endl;
        E_tmp_total += hit->getE();
    }

    for (unsigned i = 0; i < n; ++i) E += ClusterVec->at(i)->getE();
    E = E / E_Tot;

    return E;
}

double Cluster_Analysis::FindMoment(unsigned n, int type, bool center) {
    // Sanity Check
    if (n <= 0) return -1;

    // Initialize Layer vector to store hits energy in the same layer
    //            pos vector to store the hits position
    auto layer_vec = std::vector<double>();
    auto pos_vec = std::vector<double>();
    if (type != 0) { // along certain axis
        int max_layer = 0;
        for (auto hit: *ClusterVec) {
            if (type == 1) max_layer = (hit->getCellIdX() > max_layer) ? hit->getCellIdX() : max_layer;
            else if (type == 2) max_layer = (hit->getCellIdY() > max_layer) ? hit->getCellIdY() : max_layer;
            else if (type == 3) max_layer = (hit->getCellIdZ() > max_layer) ? hit->getCellIdZ() : max_layer;
        }
        for (int i = 0; i < max_layer; ++i) {
            layer_vec.push_back(0.);
            pos_vec.push_back(0.);
        }

        // if no layer, just return -1
        if (max_layer <= 0) return -1;

        // Fill hits into different layers;
        // Energy is sum
        for (auto hit: *ClusterVec) {
            // Note that cell id starts from 1
            if (type == 1) {
                layer_vec.at(hit->getCellIdX() - 1) += hit->getE();
                pos_vec.at(hit->getCellIdX() - 1) = hit->getX();
            } else if (type == 2) {
                layer_vec.at(hit->getCellIdY() - 1) += hit->getE();
                pos_vec.at(hit->getCellIdY() - 1) = hit->getY();
            } else if (type == 3) {
                layer_vec.at(hit->getCellIdZ() - 1) += hit->getE();
                pos_vec.at(hit->getCellIdZ() - 1) = hit->getZ();
            }
        }
    } else { // for all hits
        for (auto hit: *ClusterVec) {
            layer_vec.push_back(hit->getE());
            pos_vec.push_back(sqrt(pow(hit->getX(), 2) + pow(hit->getY(), 2) + pow(hit->getZ(), 2)));
        }
    }

    // convert from mm to cm
    //for (auto &i : pos_vec) i /= 10.;

    [[maybe_unused]] double sum_layer = std::accumulate(pos_vec.begin(), pos_vec.end(), 0.);
    double out_moment = 0.;
    if (!center || n == 1) {
        // Calculate the n-th power sum, weighted by energy
        out_moment = std::inner_product(pos_vec.begin(), pos_vec.end(), layer_vec.begin(), out_moment,
                                        std::plus<>(),
                                        [n](double x, double e) { return e * std::pow(x, n); });
        // Divided by total Energy to normalize weight
        out_moment /= E_Tot;
        return out_moment;
    }
    // If to calculate centralized moments
    double mean = std::inner_product(pos_vec.begin(), pos_vec.end(), layer_vec.begin(), 0.);
    mean /= E_Tot;
    // define temp variable
    std::vector<double> diff(layer_vec.size());
    // calculate distance to mean and store in the temp variable vector
    std::transform(pos_vec.begin(), pos_vec.end(), diff.begin(), [mean](double x) { return x - mean; });
    out_moment = std::inner_product(diff.begin(), diff.end(), layer_vec.begin(), out_moment,
                                    std::plus<>(),
                                    [n](double x, double e) { return e * std::pow(x, n); });
    out_moment /= E_Tot;
    return out_moment;
}

double Cluster_Analysis::FindLatMoment() {
    double center_x = FindMoment(1, 1, false);
    double center_y = FindMoment(1, 2, false);
    //Temperarily let central axis be parallel to z axis

    //too few hits
    if (ClusterVec->size() <= 2) return 0;

    double lat = 0;
    for (auto h: *ClusterVec) {
        double diffX = h->getX() - center_x;
        double diffY = h->getY() - center_y;
        lat += h->getE() * std::pow((std::pow(diffX, 2) + std::pow(diffY, 2)), 0.5);
    }
    lat /= E_Tot;
    return lat;
}

bool Cluster_Analysis::FineECellXY(std::vector<double>* ECell) { //sumZ
    if (ECell->empty()) return false;
    std::vector<double> temp;
    temp.resize(dNX()*dNY());
    for (auto hit: *ClusterVec) {
        int Xid = hit->getCellIdX() - 1;
        int Yid = hit->getCellIdY() - 1;
        temp.at(Xid + dNX() * Yid) += hit->getE(); // Y-X
    }
    std::copy(temp.begin(), temp.end(), ECell->begin());
    return true;
}

bool Cluster_Analysis::FineECellXZ(std::vector<double> *ECell) { //sumY
    if (ECell->empty()) return false;
    std::vector<double> temp;
    temp.resize(dNX()*dNZ());
    for (auto hit: *ClusterVec) {
        int Xid = hit->getCellIdX() - 1;
        int Zid = hit->getCellIdZ() - 1;
        temp.at(Xid + dNX() * Zid) += hit->getE(); //Z-X
    }
    std::copy(temp.begin(), temp.end(), ECell->begin());
    return true;
}


bool Cluster_Analysis::FineECellYZ(std::vector<double> *ECell) { //sumX
    if (ECell->empty()) return false;
    std::vector<double> temp;
    temp.resize(dNY()*dNZ());
    for (auto hit: *ClusterVec) {
        int Yid = hit->getCellIdX() - 1;
        int Zid = hit->getCellIdZ() - 1;
        temp.at(Yid + dNY() * Zid) += hit->getE(); //Z-Y
    }
    std::copy(temp.begin(), temp.end(), ECell->begin());
    return true;
}


bool Cluster_Analysis::FineECellZ(std::vector<double>* ECell) { //sumXY
    if (ECell->empty()) return false;
    std::vector<double> temp;
    temp.resize(dNZ());
    for (auto hit: *ClusterVec) {
        int Zid = hit->getCellIdZ() - 1;
        temp.at(Zid) += hit->getE(); //Z-Y
    }
    std::copy(temp.begin(), temp.end(), ECell->begin());
    return true;
}


bool Cluster_Analysis::maxXY(std::vector<int>* ECell) { //sumZ
    if (ECell->empty()) return false;
    std::vector<int> temp;
    temp.resize(dNX()*dNY());
    for (auto hit: *ClusterVec) {
        int Xid = hit->getCellIdX() - 1;
        int Yid = hit->getCellIdY() - 1;
        int Zid = hit->getCellIdZ();
        temp.at(Xid + dNX() * Yid) =max(temp.at(Xid + dNX() * Yid), Zid); // Y-X
    }
    std::copy(temp.begin(), temp.end(), ECell->begin());
    return true;
}


bool Cluster_Analysis::FineECellXYZ(std::vector<double> *ECell) { //all hits
    if (ECell->empty()) return false;
    std::vector<double> temp;
    temp.resize(dNX()*dNY()*dNZ());
    for (auto hit: *ClusterVec) {
        int Xid = hit->getCellIdX() - 1;
        int Yid = hit->getCellIdY() - 1;
        int Zid = hit->getCellIdZ() - 1;
        temp.at(Xid + dNX()*Yid + dNX()*dNY()*Zid) = hit->getE(); //X-Y-Z
    }
    std::copy(temp.begin(), temp.end(), ECell->begin());
    return true;
}

bool Cluster_Analysis::FinePosCellXYZ(std::vector<TVector3> *PosCell) {
    if (PosCell->empty()) return false;
    std::vector<TVector3> temp;
    temp.resize(dNX()*dNY()*dNZ());
    for (auto hit: *ClusterVec) {
        int Xid = hit->getCellIdX() - 1;
        int Yid = hit->getCellIdY() - 1;
        int Zid = hit->getCellIdZ() - 1;
        temp.at(Xid + dNX()*Yid + dNX()*dNY()*Zid) = {hit->getX(), hit->getY(), hit->getZ()}; //X-Y-Z
    }
    std::copy(temp.begin(), temp.end(), PosCell->begin());
    return true;
}

bool Cluster_Analysis::NXY(std::vector<int>* ECell) { //sumZ
    if (ECell->empty()) return false;
    std::vector<double> temp;
    temp.resize(dNX()*dNY());
    for (auto hit: *ClusterVec) {
        int Xid = hit->getCellIdX() - 1;
        int Yid = hit->getCellIdY() - 1;
        temp.at(Xid + dNX() * Yid) += 1; // Y-X
    }
    std::copy(temp.begin(), temp.end(), ECell->begin());
    return true;
}

bool Cluster_Analysis::Calculate_COG(std::vector<TVector3>* PosCell, std::vector<double>* ECell,
                   double& COG_X, double& COG_Y, double& COG_Z) {
    if (ECell->empty() || PosCell->empty()) return false;
    double sum_pos_x = 0.0;
    double sum_pos_y = 0.0;
    double sum_pos_z = 0.0;
    double sum_e = 0.0;

    for (size_t i = 0; i < PosCell->size(); ++i) {
        sum_pos_x += PosCell->at(i).X() * ECell->at(i);
        sum_pos_y += PosCell->at(i).Y() * ECell->at(i);
        sum_pos_z += PosCell->at(i).Z() * ECell->at(i);
        sum_e += ECell->at(i);
    }

    COG_X = (sum_e > 0.0) ? sum_pos_x / sum_e : 0.0;
    COG_Y = (sum_e > 0.0) ? sum_pos_y / sum_e : 0.0;
    COG_Z = (sum_e > 0.0) ? sum_pos_z / sum_e : 0.0;
    return true;
}

TVector3 Cluster_Analysis::PosToIndex(TVector3 PosCell)
{
    TVector3 IndexCell;
    if (PosCell.Mag() == 0) return IndexCell; 
    IndexCell.SetZ( static_cast<int>((PosCell.Z() - dAnaData->getECalSurfaceZ() - 0.5 * dAnaData->getECalCellDz()) / dAnaData->getECalCellDz()) );
    if (m_isStaggered){
        
        double Layer_CenterY = dAnaData->getECalCenterY() + (((static_cast<int>(IndexCell.Z()) % 2) == 0) ? 0.25 : -0.25) * dAnaData->getECalCellDy();
        double Layer_CenterX = dAnaData->getECalCenterX() + (((static_cast<int>(IndexCell.Z()) % 2) == 0) ? 0.25 : -0.25) * dAnaData->getECalCellDx();
        double Layer_LengthY = 0.5 * dNY() * dAnaData->getECalCellDy();
        double Layer_LengthX = 0.5 * dNX() * dAnaData->getECalCellDx();
        IndexCell.SetY( static_cast<int>((PosCell.Y() - (Layer_CenterY - Layer_LengthY)) / dAnaData->getECalCellDy()) );
        IndexCell.SetX( static_cast<int>((PosCell.X() - (Layer_CenterX - Layer_LengthX)) / dAnaData->getECalCellDx()) );
    }
    else{
        IndexCell.SetY( static_cast<int>((PosCell.Y() - (dAnaData->getECalCenterY() - 0.5 * dAnaData->getECalLengthY())) / dAnaData->getECalCellDy()) );
        IndexCell.SetX( static_cast<int>((PosCell.X() - (dAnaData->getECalCenterX() - 0.5 * dAnaData->getECalLengthX())) / dAnaData->getECalCellDx()) );
    }
    
    return IndexCell;
}

double Cluster_Analysis::GetECellNeighbor(TVector3 IndexCell, std::vector<double>* ECell, int n)
{
    double ECellNeighbor = 0;
    if (IndexCell.Mag() == 0 || ECell->empty()) return ECellNeighbor;
    n = n - 2;
    for (int i = -n; i <= n; ++i)
    {
        for (int j = -n; j <= n; ++j)
        {
            for (int k = -n; k <= n; ++k)
            {
                TVector3 _Index = IndexCell + TVector3(i, j, k);
                if (_Index.X() >= 0 && _Index.X() < dNX() &&
                    _Index.Y() >= 0 && _Index.Y() < dNY() &&
                    _Index.Z() >= 0 && _Index.Z() < dNZ())
                {
                    ECellNeighbor += ECell->at(dAnaData->getACC(_Index.X()+1, _Index.Y()+1, _Index.Z()+1));
                }
            }
        }
    }

    return ECellNeighbor;
}

int Cluster_Analysis::GetShowerStartLayer(std::vector<int>* hits_on_layer) {
    if (hits_on_layer->empty()) return -1;
    int Nhits_threshold = 3;
    int target_layer = -1;
    for (int i = 0; i < dNZ() - 2; ++i) {
        if (hits_on_layer->at(i) >= Nhits_threshold && hits_on_layer->at(i + 1) >= Nhits_threshold && hits_on_layer->at(i + 2) >= Nhits_threshold) {
            target_layer = i;
        }
    }
    return target_layer;
}

int Cluster_Analysis::GetShowerEndLayer(std::vector<int>* hits_on_layer) {
    if (hits_on_layer->empty()) return -1;
    int Nhits_threshold = 3;
    int target_layer = -1;
    for (int i = dNZ() - 1; i > 0; --i) {
        if (hits_on_layer->at(i) < Nhits_threshold && hits_on_layer->at(i - 1) < Nhits_threshold) {
            target_layer = i;
        }
    }
    return target_layer;
}

bool Cluster_Analysis::GetLayerCOGWidth(std::vector<TVector3>* COG_Layer, std::vector<TVector3>* shower_layer_width, std::vector<int>* hits_on_layer, std::vector<double>* ECell, std::vector<TVector3> *PosCell) {
    if (ECell->empty() || PosCell->empty()) return false;
    vector<double> sum_E_layer(dNZ());
    std::vector<TVector3> _COG_Layer(dNZ());
    for (size_t i = 0; i < PosCell->size(); ++i) {
        int Zid = static_cast<int>((PosCell->at(i).Z() - dAnaData->getECalSurfaceZ() - 0.5 * dAnaData->getECalCellDz()) / dAnaData->getECalCellDz());
        sum_E_layer.at(Zid) += ECell->at(i);
        _COG_Layer.at(Zid) += TVector3(PosCell->at(i).X(), PosCell->at(i).Y(), PosCell->at(i).Z()) * ECell->at(i);
    }
    for (int i = 0; i < dNZ(); ++i) {
        if (hits_on_layer->at(i) > 0) {
            COG_Layer->emplace_back(TVector3(_COG_Layer.at(i).X() / sum_E_layer.at(i), _COG_Layer.at(i).Y() / sum_E_layer.at(i), _COG_Layer.at(i).Z() / sum_E_layer.at(i)));
            _COG_Layer.at(i) = TVector3(_COG_Layer.at(i).X() / sum_E_layer.at(i), _COG_Layer.at(i).Y() / sum_E_layer.at(i), _COG_Layer.at(i).Z() / sum_E_layer.at(i));
        }
    }
    for (size_t i = 0; i < PosCell->size(); ++i) {
        int Zid = static_cast<int>((PosCell->at(i).Z() - dAnaData->getECalSurfaceZ() - 0.5 * dAnaData->getECalCellDz()) / dAnaData->getECalCellDz());
        if (hits_on_layer->at(Zid) > 0) shower_layer_width->at(Zid) += TVector3(pow(PosCell->at(i).X() - _COG_Layer.at(Zid).X(), 2) / hits_on_layer->at(Zid), pow(PosCell->at(i).Y() - _COG_Layer.at(Zid).Y(), 2) / hits_on_layer->at(Zid), 0);
    }
    for (int i = 0; i < dNZ(); ++i) {
        if (hits_on_layer->at(i) > 0) shower_layer_width->at(i) = TVector3(sqrt(shower_layer_width->at(i).X()), sqrt(shower_layer_width->at(i).Y()), 0);
    }
    return true;
}

double Cluster_Analysis::DistanceToLine(const double* par, const TVector3& point) {
    TVector3 p(par[0], par[1], par[2]);
    TVector3 d(par[3], par[4], par[5]);

    TVector3 diff = point - p;
    double t = diff.Dot(d) / d.Mag2();  
    TVector3 projection = p + t * d;  
    return (point - projection).Mag2(); 
}

// double Cluster_Analysis::SumDist2(const double* par) {
//     double sum = 0.0;
//     for (size_t i = 0; i < COG_Layer->size(); ++i) {
//         sum += DistanceToLine(par, TVector3(COG_Layer->at(i).X(), COG_Layer->at(i).Y(), COG_Layer->at(i).Z()));
//     }
//     return sum;
// }

bool Cluster_Analysis::GetRMSRadius(std::vector<TVector3>* COG_Layer, std::vector<double>* ECell, std::vector<TVector3> *PosCell, double* radius, double* radius_w) {
    if (PosCell->empty() || ECell->empty()) return false;
    ROOT::Math::Minimizer* minimizer = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");

    auto sumDist2 = [this, COG_Layer](const double* par) -> double {
        double sum = 0.0;
        for (size_t i = 0; i < COG_Layer->size(); ++i) {
            sum += DistanceToLine(par, TVector3(COG_Layer->at(i).X(), COG_Layer->at(i).Y(), COG_Layer->at(i).Z()));
        }
        return sum;
    };

    ROOT::Math::Functor f(sumDist2, 6);
    minimizer->SetFunction(f);
    minimizer->SetVariable(0, "x0", 0, 0.1);
    minimizer->SetVariable(1, "y0", 0, 0.1);
    minimizer->SetVariable(2, "z0", 0, 0.1);
    minimizer->SetVariable(3, "dx", 0, 0.1);
    minimizer->SetVariable(4, "dy", 0, 0.1);
    minimizer->SetVariable(5, "dz", 1, 0.1);
    minimizer->Minimize();
    const double* result = minimizer->X();
    
    double d2total = 0;
    double weight_sum = 0;
    double d2total_w = 0;
    double weight_sum_w = 0;
    for (size_t i = 0; i < PosCell->size(); ++i) {
        d2total += ECell->at(i) * DistanceToLine(result, PosCell->at(i));
        weight_sum += ECell->at(i);
        d2total_w += DistanceToLine(result, PosCell->at(i));
        weight_sum_w += 1;
    }
    *radius = sqrt(d2total / weight_sum);
    *radius_w = sqrt(d2total_w / weight_sum_w);

    delete minimizer;
    return true;
}

double Cluster_Analysis::GetShowerDensity(std::vector<TVector3>* IndexCell, std::vector<double>* ECell, double ECAL_Hit_Threshold, int n) {
    if (IndexCell->empty()) return -1;
    n = n - 2;
    double shower_density = 0;
    for (size_t I = 0; I < IndexCell->size(); ++I) {
        if (IndexCell->at(I).Mag() == 0) continue;
        for (int i = -n; i <= n; ++i)
        {
            for (int j = -n; j <= n; ++j)
            {
                for (int k = -n; k <= n; ++k)
                {
                    TVector3 _Index = IndexCell->at(I) + TVector3(i, j, k);
                    if (_Index.X() >= 0 && _Index.X() < dNX() &&
                        _Index.Y() >= 0 && _Index.Y() < dNY() &&
                        _Index.Z() >= 0 && _Index.Z() < dNZ() &&
                        ECell->at(dAnaData->getACC(_Index.X()+1, _Index.Y()+1, _Index.Z()+1)) > ECAL_Hit_Threshold)
                    {
                        shower_density++;
                    }
                }
            }
        }
    }
    return shower_density / IndexCell->size();
}

int Cluster_Analysis::NewScale(std::vector<TVector3>* PosCell, std::vector<TVector3>* IndexCell, int ratio_x, int ratio_y, int ratio_z) {
    if (PosCell->empty() || IndexCell->empty()) return -1;
    std::unordered_map<int, int> ID_hit_map;
    int nhits = PosCell->size();
    for (int i = 0; i < nhits; ++i) {
        int supercellID_x =  static_cast<int>(IndexCell->at(i).X() / ratio_x);
        int supercellID_y =  static_cast<int>(IndexCell->at(i).Y() / ratio_y);
        int supercellID_z =  static_cast<int>(IndexCell->at(i).Z() / ratio_z);

        int index_new = dAnaData->getACC(supercellID_x + 1, supercellID_y + 1, supercellID_z + 1);
        ++ID_hit_map[index_new];
    }
    int nhits_new = ID_hit_map.size();
    ID_hit_map.clear();
    return nhits_new;
}

bool Cluster_Analysis::GetFD2D(std::vector<TVector3>* PosCell, std::vector<TVector3>* IndexCell, double* mean, double* rms) {
    if (PosCell->empty()) return false;
    std::vector<int> scale = {2, 3, 4, 5, 6, 8, 10, 12, 14, 16, 18, 20};
    const int num = scale.size();
    std::vector<double> fd(num);
    std::vector<int> nhits_new(num);
    int nhits = PosCell->size();
    for (int i = 0; i < num; ++i) {
        nhits_new.at(i) = NewScale(PosCell, IndexCell, scale.at(i), scale.at(i), 1);
        if (nhits == 0 || nhits_new.at(i) <= 0) {
            fd.at(i) = -1.0;
            continue;
        }
        fd.at(i) = std::log(static_cast<double>(nhits) / nhits_new.at(i)) / std::log(scale.at(i));
    }
    *mean = fd.empty() ? 0. : std::accumulate(fd.begin(), fd.end(), 0.0) / fd.size();
    *rms = fd.empty() ? 0. : std::sqrt(std::accumulate(fd.begin(), fd.end(), 0.0,
                                [](double sum, double value) { return sum + value * value; }) / fd.size());
    return true;
}

bool Cluster_Analysis::GetFD3D(std::vector<TVector3>* PosCell, std::vector<TVector3>* IndexCell, double* mean, double* rms) {
    if (PosCell->empty()) return false;
    std::vector<int> scale = {2, 3, 4, 5, 6, 8, 10, 12, 14, 16, 18, 20};
    const int num = scale.size();
    std::vector<double> fd(num);
    std::vector<int> nhits_new(num);
    int nhits = PosCell->size();
    for (int i = 0; i < num; ++i) {
        nhits_new.at(i) = NewScale(PosCell, IndexCell, scale.at(i), scale.at(i), scale.at(i));
        if (nhits == 0 || nhits_new.at(i) <= 0) {
            fd.at(i) = -1.0;
            continue;
        }
        fd.at(i) = std::log(static_cast<double>(nhits) / nhits_new.at(i)) / std::log(scale.at(i));
    }
    *mean = fd.empty() ? 0. : std::accumulate(fd.begin(), fd.end(), 0.0) / fd.size();
    *rms = fd.empty() ? 0. : std::sqrt(std::accumulate(fd.begin(), fd.end(), 0.0,
                                [](double sum, double value) { return sum + value * value; }) / fd.size());
    return true;
}