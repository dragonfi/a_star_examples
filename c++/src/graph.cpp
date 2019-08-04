#include <ostream>
#include <vector>
#include "graph.hpp"
#include "types.hpp"
#include "vec2.hpp"

namespace pathing {
    IndexedGraph::IndexedGraph(std::vector<Vec2> nodes): nodes(nodes) {
        edges.resize(nodes.size(), {});
    }
    void IndexedGraph::addEdge(Edge<Index> edge) {
        edges[edge.source].push_back(RawEdge{edge.dest, edge.weight});
    }
    std::vector<RawEdge> IndexedGraph::edgesFrom(Index node) const {
        return edges[node];
    }
    Weight IndexedGraph::distance(Index node1, Index node2) const {
        return euclidean_distance(nodes[node1], nodes[node2]);
    }
    size_t IndexedGraph::nodeCount() const {
        return nodes.size();
    };
}
