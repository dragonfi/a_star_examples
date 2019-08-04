#pragma once
#include <ostream>
#include "ostream_helpers/ostream_helpers.hpp"

#include "types.hpp"
#include "graph.hpp"

namespace pathing {
    using namespace ostream_helpers;

    struct Path {
        Weight weight;
        std::vector<Index> nodes;
        
        friend std::ostream& operator<<(std::ostream &output, const Path& path);
    };

    using Candidate = std::pair<Index, Path>;

    class AStar {
    public:
        AStar(IndexedGraph graph);
        Path shortest_path(Index source, Index dest);
    private:
        IndexedGraph graph;
        void sortAndPruneCandidates(std::vector<Candidate>& candidates, Index dest);
    };
}