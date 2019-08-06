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

    IndexedGraph::IndexedGraph(std::vector<Vec2> nodes): m_nodes(nodes) {
        m_edges.resize(nodes.size(), {});
    }

    void IndexedGraph::addEdge(Edge edge) {
        m_edges[edge.source].push_back(edge);
    }
    std::vector<Edge> IndexedGraph::edgesFrom(Index node) const {
        return m_edges[node];
    }
    Weight IndexedGraph::distance(Index node1, Index node2) const {
        return euclidean_distance(m_nodes[node1], m_nodes[node2]);
    }
    Index IndexedGraph::nodeCount() const {
        return m_nodes.size();
    }
    const std::vector<Vec2>& IndexedGraph::nodes() const {
        return m_nodes;
    }
    const std::vector<Edge> IndexedGraph::edges() const {
        std::vector<Edge> result_edges;
        for (auto edges_of_node : m_edges) {
            for (auto single_edge : edges_of_node) {
                result_edges.push_back(single_edge);
            }
        }
        return result_edges;
    }
}
