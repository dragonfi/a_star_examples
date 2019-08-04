#include<vector>
#include<cmath>
#include<iostream>
#include<algorithm>

template<class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
    out << "[";
    size_t last = v.size() - 1;
    for(size_t i = 0; i < v.size(); ++i) {
        out << v[i];
        if (i != last) {
            out << ", ";
        }
    }
    out << "]";
    return out;
}

template<class T1, class T2>
std::ostream& operator<<(std::ostream& out, const std::pair<T1, T2>& v) {
    out << "(" << v.first << ", " << v.second << ")";
    return out;
}

using Index = size_t;
using Weight = double;

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

template<class Key>
struct Path {
    Weight weight;
    std::vector<Key> nodes;
    
    friend std::ostream &operator<<(std::ostream &output, const Path<Key>& path) {
        output << "Path(" << path.weight << ": " << path.nodes << ")";
        return output;
    }
};

struct Vec2 {
    double x;
    double y;
};

Weight euclidean_distance(Vec2 v1, Vec2 v2) {
    Weight dx = v2.x - v1.x;
    Weight dy = v2.y - v1.y;
    return sqrt(dx * dx + dy * dy);
}

class IndexedGraph {
private:
    struct RawEdge {
        Index dest;
        Weight weight;
    };
public:
    IndexedGraph(std::vector<Vec2> nodes): nodes(nodes) {
        edges.resize(nodes.size(), {});
    }
    void addEdge(Edge<Index> edge) {
        edges[edge.source].push_back(RawEdge{edge.dest, edge.weight});
    }
    std::vector<RawEdge> edgesFrom(Index node) const {
        return edges[node];
    }
    Weight distance(Index node1, Index node2) const {
        return euclidean_distance(nodes[node1], nodes[node2]);
    }
    size_t nodeCount() {
        return nodes.size();
    };
private:
    std::vector<std::vector<RawEdge>> edges;
    std::vector<Vec2> nodes;

};

class AStar {
    using Index = size_t;
    using Candidate = std::pair<Index, Path<Index>>;
public:
    AStar(IndexedGraph graph): graph(graph) {};

    Path<Index> shortest_path(Index source, Index dest) {
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
private:
    IndexedGraph graph;

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

    void sortAndPruneCandidates(std::vector<Candidate>& candidates, Index dest) {
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
};

double randomDouble(double min, double max) {
    return min + max * (rand() / static_cast<double>(RAND_MAX));
}

std::vector<Vec2> randomPoints(size_t count, Vec2 min, Vec2 max) {
    std::vector<Vec2> points;
    for(size_t i = 0; i < count; ++i) {
        points.push_back({randomDouble(min.x, max.x), randomDouble(min.y, max.y)});
    }
    return points;
}

IndexedGraph connectPointsWithinThreshold(const std::vector<Vec2>& points, double threshold) {
    IndexedGraph graph(points);

    for(size_t i = 0; i < points.size(); ++i) {
        for(size_t j = 0; j < points.size(); ++j) {
            double distance = euclidean_distance(points[i], points[j]);
            if (distance == 0) {
                continue;
            }
            if (distance < threshold) {
                graph.addEdge(Edge<size_t>{i, j, distance});
            }
        }
    }
    return graph;
}

int main() {
    auto points = randomPoints(1000, {0, 0}, {100, 100});
    auto graph = connectPointsWithinThreshold(points, 5);
    AStar aStar(graph);
    std::cout << aStar.shortest_path(700, 500) << std::endl;
    for(size_t j = 0; j < points.size(); j++) {
        //std::cout << j << " ";
        auto path = aStar.shortest_path(0, j);
        //std::cout << path.nodes.size() << std::endl;
    }

    size_t sum = 0;
    for(size_t i = 0; i < points.size(); i++) {
        sum += graph.edgesFrom(i).size();
    }
    std::cout << points.size() << " " << sum << std::endl;
    
    return 0;
}