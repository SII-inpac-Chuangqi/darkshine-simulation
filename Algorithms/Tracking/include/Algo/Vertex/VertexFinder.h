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
#include "Algo/TypeDef.h"
#include "Algo/Object/TrkHit.h"
#include "Algo/Object/DTrack.h"
#include "Algo/Vertex/DVertex.h"
#include "Algo/Utils/Clusterer.h"

class VertexFinder
{
public:
    VertexFinder() {}
    VertexFinder(std::vector<std::shared_ptr<DTrack>> *tracks);
    void FindVertexes(std::vector<std::shared_ptr<DVertex>> *vertexes);

    ~VertexFinder() {vertexes_ = nullptr; tracks_ = nullptr;}

private:
    class Stair
    {
    public:
        Stair() = delete;
        Stair(int stair_no) : stair_no_(stair_no) {}
        ~Stair() {}

        const int stair_no_;
        double stair_height_{0.};
        std::vector<std::array<double, 3>>   splits_;
        std::vector<std::shared_ptr<DTrack>> slabs_;
    };

    void BuildSpiralStaircase();
    void FindClusterInStair(const std::shared_ptr<Stair> &stair);

    std::vector<std::shared_ptr<DVertex>> *vertexes_{nullptr};

    std::vector<std::shared_ptr<DTrack>> *tracks_{nullptr};
    std::map<int, std::shared_ptr<Stair>> spiral_staircase_;
};

#endif // VERTEX_FINDER_H
