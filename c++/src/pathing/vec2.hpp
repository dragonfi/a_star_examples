#pragma once

#include "types.hpp"

namespace pathing {
    struct Vec2 {
        double x;
        double y;
    };

    Weight euclidean_distance(Vec2 v1, Vec2 v2);
}