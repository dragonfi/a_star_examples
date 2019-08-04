#include <cmath>
#include <vector>
#include "randomPoints.hpp"
#include "graph.hpp"

namespace pathing {
    double randomDouble(double min, double max) {
        return min + max * (rand() / static_cast<double>(RAND_MAX));
    }

    std::vector<Vec2> randomPoints(Index count, Vec2 min, Vec2 max) {
        std::vector<Vec2> points;
        for(Index i = 0; i < count; ++i) {
            points.push_back({randomDouble(min.x, max.x), randomDouble(min.y, max.y)});
        }
        return points;
    }

    IndexedGraph connectPointsWithinThreshold(const std::vector<Vec2>& points, double threshold) {
        IndexedGraph graph(points);

        for(Index i = 0; i < points.size(); ++i) {
            for(Index j = 0; j < points.size(); ++j) {
                double distance = euclidean_distance(points[i], points[j]);
                if (distance == 0) {
                    continue;
                }
                if (distance < threshold) {
                    graph.addEdge(Edge<Index>{i, j, distance});
                }
            }
        }
        return graph;
    }
}
