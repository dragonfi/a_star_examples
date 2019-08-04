#include <cmath>
#include <algorithm>
#include "a_star.hpp"

namespace pathing {
    struct FirstIsLargerWithDistance {
        const IndexedGraph& graph;
        Index dest;

        bool operator()(const Candidate& c1, const Candidate& c2)
        {
            Weight c1Weight = c1.second.weight + graph.distance(c1.first, dest);
            Weight c2Weight = c2.second.weight + graph.distance(c2.first, dest);
            //std::cout << c1.first << " " << c2.first << " " << c1Weight << " " << c2Weight << std::endl;
            return c1Weight > c2Weight;
        }
    };

    using Candidate = std::pair<Index, Path<Index>>;
    AStar::AStar(IndexedGraph graph): graph(graph) {};

    Path<Index> AStar::shortest_path(Index source, Index dest) {
        std::vector<std::pair<bool, Path<Index>>> explored(graph.nodeCount(), {false, {MAXFLOAT, {}}});
        std::vector<Candidate> candidates;
        candidates.push_back({source, {0, {source}}});

        while (!candidates.empty()) {
            sortAndPruneCandidates(candidates, dest);
            //std::cout << candidates << std::endl;
            //std::cout << candidates.size() << std::endl;
            Candidate candidate = candidates.back();
            candidates.pop_back();
            Index node = candidate.first;
            Path<Index> path = candidate.second;
            if (node == dest) {
                return path;
            }
            if (explored[node].first || explored[node].second.weight > path.weight) {
                explored[node] = {true, path};
            }
            //std::cout << graph.edgesFrom(node).size() << std::endl;
            for(const auto& edge: graph.edgesFrom(node)) {
                if (explored[edge.dest].first) {
                    continue;
                }
                std::vector<Index> new_nodes = path.nodes;
                new_nodes.push_back(edge.dest);
                candidates.push_back({edge.dest, {path.weight + edge.weight, new_nodes}});
            }
        }
        return {0, {}};
    }

    void AStar::sortAndPruneCandidates(std::vector<Candidate>& candidates, Index dest) {
        auto cmp = FirstIsLargerWithDistance{graph, dest};
        std::sort(candidates.begin(), candidates.end(), cmp);

        std::vector<bool> visitedNodes(graph.nodeCount(), false);

        auto iter = candidates.begin();
        while(iter != candidates.end()) {
            if (visitedNodes[iter->first]) {
                iter = candidates.erase(iter);
            } else {
                visitedNodes[iter->first] = iter->second.weight;
                ++iter;
            }
        }
    }
}