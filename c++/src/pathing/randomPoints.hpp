#pragma once
#include <vector>

#include "types.hpp"
#include "vec2.hpp"
#include "graph.hpp"

namespace pathing {
    double randomDouble(double min, double max);
    std::vector<Vec2> randomPoints(size_t count, Vec2 min, Vec2 max);
    IndexedGraph connectPointsWithinThreshold(const std::vector<Vec2>& points, double threshold);
}