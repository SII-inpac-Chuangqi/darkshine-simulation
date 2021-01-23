//
// Credit to Lv Meng
//

#include "Algo/Trk_LineFit.h"

std::pair<V3, V3> Trk_LineFit::best_line_from_points() {
    // copy coordinates to  matrix in Eigen format
    size_t num_atoms = Points.size();
    Eigen::Matrix<V3::Scalar, Eigen::Dynamic, Eigen::Dynamic> centers(num_atoms, 3);
    for (size_t i = 0; i < num_atoms; ++i) centers.row(i) = Points[i];

    V3 origin = centers.colwise().mean();
    Eigen::MatrixXd centered = centers.rowwise() - origin.transpose();
    Eigen::MatrixXd cov = centered.adjoint() * centered;
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eig(cov);
    V3 axis = eig.eigenvectors().col(2).normalized();

    return std::make_pair(origin, axis);
}

template<class Vector3>
std::pair<Vector3, Vector3> Trk_LineFit::best_line_from_points(const std::vector<Vector3> &c) {
    // copy coordinates to  matrix in Eigen format
    size_t num_atoms = c.size();
    Eigen::Matrix<typename Vector3::Scalar, Eigen::Dynamic, Eigen::Dynamic> centers(num_atoms, 3);
    for (size_t i = 0; i < num_atoms; ++i) centers.row(i) = c[i];

    Vector3 origin = centers.colwise().mean();
    Eigen::MatrixXd centered = centers.rowwise() - origin.transpose();
    Eigen::MatrixXd cov = centered.adjoint() * centered;
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eig(cov);
    Vector3 axis = eig.eigenvectors().col(2).normalized();

    return std::make_pair(origin, axis);
}

void Trk_LineFit::FillPoints(std::vector<double> x, std::vector<double> y, std::vector<double> z) {
    assert(x.size() == y.size() == z.size());
    int N = x.size();
    Eigen::VectorXd vx = Eigen::Map<Eigen::VectorXd>(x.data(), N);
    Eigen::VectorXd vy = Eigen::Map<Eigen::VectorXd>(y.data(), N);
    Eigen::VectorXd vz = Eigen::Map<Eigen::VectorXd>(z.data(), N);
    Eigen::MatrixXd M(vx.rows(), 3);
    M << vx, vy, vz;
    for (int i = 0; i < N; ++i) Points.push_back(M.row(i));
}

void Trk_LineFit::AddPoint(double x, double y, double z) {
    Eigen::MatrixXd M(1, 3);
    M << x, y, z;
    Points.push_back(M.row(0));
}

void Trk_LineFit::Clean() {
    Points.clear();
    Points.shrink_to_fit();
    pointsWithEnergy.clear();
    pointsWithEnergy.shrink_to_fit();
}

void Trk_LineFit::SortZ() {
    V4 temp;
    for (unsigned i = 0; i < pointsWithEnergy.size(); ++i) {
        for (unsigned j = 0; j < i; ++j) {
            if (pointsWithEnergy[j][4] > pointsWithEnergy[j + 1][4]) {
                temp = pointsWithEnergy[j];
                pointsWithEnergy[j] = pointsWithEnergy[j + 1];
                pointsWithEnergy[j + 1] = temp;
            }
        }
    }
}

int Trk_LineFit::xyRecon() {
    double E, Wx, Wy;
    if (pointsWithEnergy.empty()) return 0;
    for (auto it = pointsWithEnergy.begin(); it < pointsWithEnergy.end(); ++it) {
        E = Wx = Wy = 0;
        do {
            E += (*it)[0];
            Wx += (*it)[0] * (*it)[1];
            Wy += (*it)[0] * (*it)[2];
            ++it;
        } while (it <= pointsWithEnergy.end() && (*it)[3] == (*(it - 1))[3]);
        AddPoint(Wx / E, Wy / E, (*(it - 1))[3]);
    }
    return Points.size();
}


