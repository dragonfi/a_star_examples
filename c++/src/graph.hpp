#pragma once
#include <ostream>
#include <vector>
#include "types.hpp"
#include "vec2.hpp"

namespace pathing {
    template<class Key>
    struct Edge {
        Key source;
        Key dest;
        Weight weight;

        friend std::ostream &operator<<(std::ostream &output, const Edge<Key>& edge) {
            output << edge.source << " -> " << edge.dest << " : " << edge.weight;
            return output;
        }
    };
    struct RawEdge {
        Index dest;
        Weight weight;
    };

    class IndexedGraph {
    public:
        IndexedGraph(std::vector<Vec2> nodes);
        void addEdge(Edge<Index> edge);
        std::vector<RawEdge> edgesFrom(Index node) const;
        Weight distance(Index node1, Index node2) const;
        size_t nodeCount() const;
    private:
        std::vector<std::vector<RawEdge>> edges;
        std::vector<Vec2> nodes;
    };
}