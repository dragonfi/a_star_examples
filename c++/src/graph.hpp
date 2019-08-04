#pragma once
#include <ostream>
#include <vector>
#include "types.hpp"
#include "vec2.hpp"

namespace pathing {

    struct Edge {
        Index source;
        Index dest;
        Weight weight;

        friend std::ostream &operator<<(std::ostream &output, const Edge& edge);
    };

    class IndexedGraph {
    public:
        IndexedGraph(std::vector<Vec2> nodes);
        void addEdge(Edge edge);
        std::vector<Edge> edgesFrom(Index node) const;
        Weight distance(Index node1, Index node2) const;
        Index nodeCount() const;
    private:
        std::vector<std::vector<Edge>> edges;
        std::vector<Vec2> nodes;
    };
}