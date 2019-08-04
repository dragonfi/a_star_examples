#include <cmath>

#include "types.hpp"
#include "vec2.hpp"

namespace pathing {
    Weight euclidean_distance(Vec2 v1, Vec2 v2) {
        Weight dx = v2.x - v1.x;
        Weight dy = v2.y - v1.y;
        return sqrt(dx * dx + dy * dy);
    }
}
