//
// Created by Yulei on 2023/6/22.
//

#ifndef DSIMU_GNN_DIGITIZATION_H
#define DSIMU_GNN_DIGITIZATION_H

#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

#include "Event/AnaEvent.h"

// Struct to represent a point in 3D space
struct Point3D {
    float x, y, z;

    SimulatedHit *hit{};

    // Constructors
    explicit Point3D(float vx = 0.0, float vy = 0.0, float vz = 0.0) : x(vx), y(vy), z(vz) {}

    // Overloaded operators for point-wise arithmetic
    Point3D &operator+=(const Point3D &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Point3D operator*(float val) const {
        return Point3D(x * val, y * val, z * val);
    }

    Point3D operator/(float val) const {
        return Point3D(x / val, y / val, z / val);
    }

    Point3D operator+(const Point3D &other) const {
        return Point3D(x + other.x, y + other.y, z + other.z);
    }

    // Method to calculate Euclidean distance to another point
    [[nodiscard]] float distanceTo(const Point3D &other) const {
        return std::hypot(x - other.x, y - other.y, z - other.z);
    }
};


class GNN_Digitization {
public:
    static std::vector<Point3D> meanShift(const std::vector<Point3D> &points, float band_width, float epsilon);

    static SimulatedHitVec cluster(const std::vector<Point3D> &points, float tolerance, float min_energy);

    static SimulatedHitVec Run(
            SimulatedHitVec *hits1, SimulatedHitVec *hits2,
            bool if_tag_or_rec, float min_energy = 0.0f
    );

private:
    // Bandwidth parameter for the Gaussian kernel
    static constexpr float BANDWIDTH = 1.0f;

    // Threshold for convergence
    static constexpr float EPSILON = 0.1f;

    // Gaussian kernel function
    static float gaussianKernel(float distance, float bandwidth) {
        return std::exp(-0.5f * std::pow(distance / bandwidth, 2.0f));
    }
};


#endif //DSIMU_GNN_DIGITIZATION_H
