#pragma once
#include <ostream>
#include "ostream_helpers.hpp"
#include "types.hpp"
#include "graph.hpp"

namespace pathing {
    using namespace ostream_helpers;

    template<class Key>
    struct Path {
        Weight weight;
        std::vector<Key> nodes;
        
        friend std::ostream &operator<<(std::ostream &output, const Path<Key>& path) {
            output << "Path(" << path.weight << ": " << path.nodes << ")";
            return output;
        }
    };

    using Candidate = std::pair<Index, Path<Index>>;

    class AStar {
    public:
        AStar(IndexedGraph graph);
        Path<Index> shortest_path(Index source, Index dest);
    private:
        IndexedGraph graph;
        void sortAndPruneCandidates(std::vector<Candidate>& candidates, Index dest);
    };
}