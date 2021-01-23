//
// Credit to Lv Meng
//

#ifndef DSIMU_TRK_LINEFIT_H
#define DSIMU_TRK_LINEFIT_H

#include <vector>
#include <utility>
#include <algorithm>

// Eigen
#include <Eigen/Core>
#include <Eigen/Dense>

typedef Eigen::Vector3d V3;
typedef Eigen::Vector4d V4;
typedef std::vector<Eigen::Vector3d> VecV3;
typedef std::vector<Eigen::Vector4d> VecV4;

class Trk_LineFit {
public:
    Trk_LineFit() = default;

    ~Trk_LineFit() = default;

    void SortZ(); //bubbleSort
    void FillPoints(std::vector<double> x, std::vector<double> y, std::vector<double> z);

    void AddPoint(double x, double y, double z);

    int xyRecon(); //Reconstruct one point on each layer and return point number

    void Clean();

    std::pair<V3, V3> best_line_from_points();

    template<class T>
    std::pair<T, T> best_line_from_points(const std::vector<T> &c);

    VecV4 pointsWithEnergy;
private:
    VecV3 Points;
};

#endif //DSIMU_TRK_LINEFIT_H
