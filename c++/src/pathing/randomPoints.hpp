#pragma once
#include <vector>
#include "types.hpp"
#include "graph.hpp"

namespace pathing { struct Vec2; }

namespace pathing {
    double randomDouble(double min, double max);
    std::vector<Vec2> randomPoints(Index count, Vec2 min, Vec2 max);
    IndexedGraph connectPointsWithinThreshold(const std::vector<Vec2>& points, double threshold);
}