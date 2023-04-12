#ifndef VERTEX_FINDER_H
#define VERTEX_FINDER_H

#include <vector>
#include <memory>

#include "Algo/DTrack.h"

class VertexFinder
{
public:
    VertexFinder() {}
    VertexFinder(const std::vector<std::shared_ptr<DTrack>> &tracks) {}

    ~VertexFinder() {}

private:
};

#endif
