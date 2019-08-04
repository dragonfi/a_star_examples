#include <ostream>
#include <vector>
#include "graph.hpp"
#include "types.hpp"
#include "vec2.hpp"

namespace pathing {
    std::ostream &operator<<(std::ostream &output, const Edge& edge) {
        output << edge.source << " -> " << edge.dest << " : " << edge.weight;
        return output;
    }

    IndexedGraph::IndexedGraph(std::vector<Vec2> nodes): nodes(nodes) {
        edges.resize(nodes.size(), {});
    }

    void IndexedGraph::addEdge(Edge edge) {
        edges[edge.source].push_back(edge);
    }
    std::vector<Edge> IndexedGraph::edgesFrom(Index node) const {
        return edges[node];
    }
    Weight IndexedGraph::distance(Index node1, Index node2) const {
        return euclidean_distance(nodes[node1], nodes[node2]);
    }
    size_t IndexedGraph::nodeCount() const {
        return nodes.size();
    };
}
