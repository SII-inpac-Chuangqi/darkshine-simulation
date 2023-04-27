#ifndef VERTEX_FINDER_H
#define VERTEX_FINDER_H

//................................................................................//
//C++
#include <map>
#include <array>
#include <vector>
#include <memory>

//................................................................................//
//TRACKING
#include "Algo/DTrack.h"
#include "Algo/Vertex/DVertex.h"
#include "Algo/Vertex/Cluster.h"

class VertexFinder
{
public:
    VertexFinder() {}
    VertexFinder(const std::vector<std::shared_ptr<DTrack>> &tracks);
    void FindVertexes();

    ~VertexFinder() {}

private:
    class Stair
    {
    public:
        Stair() = delete;
        Stair(int stair_no) : stair_no_(stair_no) {}
        ~Stair() {}

        const int stair_no_;
        std::vector<std::array<double, 3>>   splits_;
        std::vector<std::shared_ptr<DTrack>> slabs_;
    };

    void BuildSpiralStaircase();

    std::vector<std::shared_ptr<DTrack>> tracks_;
    std::map<int, std::shared_ptr<Stair>> spiral_staircase_;
};

#endif // VERTEX_FINDER_H
