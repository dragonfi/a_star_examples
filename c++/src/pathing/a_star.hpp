#pragma once
#include <ostream>
#include <utility>
#include <vector>

#include "types.hpp"
#include "graph.hpp"

namespace pathing {

    struct Path {
        Weight weight;
        std::vector<Index> nodes;
        friend std::ostream& operator<<(std::ostream &output, const Path& path);
    };

    using Candidate = std::pair<Index, Path>;

    struct PathWithMeta {
        PathWithMeta(
            Path path_,
            std::vector<std::pair<bool, Path>> explored_,
            std::vector<Candidate> candidates_);

        Path path;
        std::vector<Index> explored;
        std::vector<Index> candidates;
    };


    class AStar {
    public:
        AStar(IndexedGraph graph);
        Path shortest_path(Index source, Index dest);
        PathWithMeta shortest_path_with_metadata(Index source, Index dest, bool skip = false);
    private:
        IndexedGraph graph;
        void sortAndPruneCandidates(std::vector<Candidate>& candidates, Index dest);
    };
}