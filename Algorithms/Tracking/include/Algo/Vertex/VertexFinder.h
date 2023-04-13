#ifndef VERTEX_FINDER_H
#define VERTEX_FINDER_H

#include <vector>
#include <memory>

#include "Algo/DTrack.h"
#include "Algo/Vertex/DVertex.h"
#include "Algo/Vertex/Cluster.h"

class VertexFinder
{
public:
    VertexFinder() {}
    VertexFinder(const std::vector<std::shared_ptr<DTrack>> &tracks);

    ~VertexFinder() {}

private:
    std::vector<std::shared_ptr<DTrack>> tracks_;
};

#endif
