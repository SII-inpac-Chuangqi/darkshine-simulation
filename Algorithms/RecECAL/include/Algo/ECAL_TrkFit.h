//
// Credit to Lv Meng
//

#ifndef DSIMU_ECAL_TRKFIT_H
#define DSIMU_ECAL_TRKFIT_H

#include <vector>
#include <utility>
#include <algorithm>

// Eigen
#include <Core>
#include <Dense>

typedef Eigen::Vector3d V3;
typedef std::vector<Eigen::Vector3d> VecV3;

class ECAL_TrkFit {
public:
    ECAL_TrkFit() = default;

    ~ECAL_TrkFit() = default;

    void FillPoints(std::vector<double> x, std::vector<double> y, std::vector<double> z);
    void AddPoint(double x, double y, double z);

    void Clean();

    std::pair<V3 , V3 > best_line_from_points();

    template<class T>
    std::pair<T, T> best_line_from_points(const std::vector<T> &c);

private:
    VecV3 Points;
};

#endif //DSIMU_ECAL_TRKFIT_H
