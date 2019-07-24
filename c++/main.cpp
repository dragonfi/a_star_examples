#include<string>
#include<map>
#include<vector>
#include<ostream>
#include<iostream>
#include<sstream>
#include<algorithm>
#include<cmath>
#include<functional>
#include<cstdlib>

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
}

template<class T1, class T2>
std::ostream& operator<<(std::ostream& out, const std::pair<T1, T2>& v) {
    out << "(" << v.first << ", " << v.second << ")";
}

template<class Key, class Any>
std::vector<Key> getMapKeys(std::map<Key, Any> m) {
    std::vector<Key> keys;
    for (auto const& item: m) {
        keys.push_back(item.first);
    }
    return keys;
}

using Weight = double;

template<class Key>
struct Edge {
    Key source;
    Key dest;
    Weight weight;

    friend std::ostream &operator<<(std::ostream &output, const Edge<Key>& edge) {
        output << edge.source << " -> " << edge.dest << " : " << edge.weight; 
    }
};

template<class Key>
struct Path {
    Weight weight;
    std::vector<Key> nodes;
    
    friend std::ostream &operator<<(std::ostream &output, const Path<Key>& path) {
        output << "Path(" << path.weight << ": " << path.nodes << ")"; 
    }
};

template<class Key, class Data>
class Graph {
public:
    void addNode(const Key& k, const Data& d) {
        if (edges.find(k) != edges.end()) {
            throw std::invalid_argument("Key already exists:" + k);
        };
        edges[k] = {};
        keyToData[k] = d;
    }
    
    void addEdge(const Key& k1, const Key& k2, const Weight& weight) {
        if(edges.find(k1) == edges.end() || edges.find(k2) == edges.end()) {
            std::stringstream s;
            s << "Some of the following keys are invalid: " << k1 << ", " << k2;
            throw std::invalid_argument(s.str());
        }
        edges[k1].push_back({k1, k2, weight});
    }

    const Data& getData(const Key& key) const {
        return keyToData.at(key);
    }

    const std::string reprEdges() {
        std::stringstream s;
 
        for(auto const& edgesForNode: edges) {
            for(auto const& edge: edgesForNode.second) {
                s << edge << std::endl;
            }
        }       
        return s.str();
    }

    const std::vector<Edge<Key>> edgesFrom(Key key) const {
        return edges.at(key);
    }

    friend std::ostream &operator<<(std::ostream &output, const Graph<Key, Data>& graph) {
        std::vector<Key> keys = getMapKeys(graph.edges);
        output << "Graph(" << keys << ")";
        return output;
    }

private:
    std::map<Key, Data> keyToData;
    using Edges = std::vector<Edge<Key>>;
    std::map<Key, Edges> edges;
};


template<class Key, class Data>
class AStar {
public:
    using Heuristic = Weight (*)(Data, Data);
    using Candidate = std::pair<Key, Path<Key>>;
    AStar(Heuristic heuristic): heuristic(heuristic) {};

    Path<Key> shortest_path(const Graph<Key, Data>& graph, Key source, Key dest) {
        std::map<Key, Path<Key>> explored;
        std::vector<Candidate> candidates;
        candidates.push_back({source, {0, {source}}});

        while (!candidates.empty()) {
            sortAndPruneCandidates(candidates, graph, dest);
            //std::cout << candidates << std::endl;
            Candidate candidate = candidates.back();
            candidates.pop_back();
            Key node = candidate.first;
            Path<Key> path = candidate.second;
            if (node == dest) {
                return path;
            }
            if (explored.find(node) == explored.end() || explored[node].weight > path.weight) {
                explored[node] = path;
            }

            for(const auto& edge: graph.edgesFrom(node)) {
                if ((explored.find(edge.dest)) != explored.end()) {
                    continue;
                }
                std::vector<Key> new_nodes = path.nodes;
                new_nodes.push_back(edge.dest);
                candidates.push_back({edge.dest, {path.weight + edge.weight, new_nodes}});
            }
        }
        return {0, {}};
    }
private:
    Heuristic heuristic;

    struct FirstIsLargerWithHeuristic {
        Heuristic heuristic;
        const Graph<Key, Data>& graph;
        Key dest;

        bool operator()(const Candidate& c1, const Candidate& c2)
        {
            Weight c1Weight = c1.second.weight;
            Weight c2Weight = c2.second.weight;
            Data c1Data = graph.getData(c1.first);
            Data c2Data = graph.getData(c2.first);
            Data destData = graph.getData(dest);
            return c1Weight + heuristic(c1Data, destData) > c2Weight + heuristic(c2Data, destData); 
        }
    };

    void sortAndPruneCandidates(std::vector<Candidate>& candidates, const Graph<Key, Data>& graph, Key dest) {
        auto cmp = FirstIsLargerWithHeuristic{heuristic, graph, dest};
        std::sort(candidates.begin(), candidates.end(), cmp);
        
        Key last_key;
        auto iter = candidates.begin();
        while(iter != candidates.end()) {
            if (last_key == iter->first) {
                iter = candidates.erase(iter);
            } else {
                last_key = iter->first;
                ++iter;
            }
        }
    }
};

template<class Data>
Weight zero_distance(Data d1, Data d2) {
    return 0;
}

struct Vec2 {
    double x;
    double y;
};

Weight euclidean_distance(Vec2 v1, Vec2 v2) {
    Weight dx = v2.x - v1.x;
    Weight dy = v2.y - v1.y;
    return sqrt(dx * dx + dy * dy);
}

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

Graph<size_t, Vec2> connectPoints(const std::vector<Vec2>& points, double threshold) {
    Graph<size_t, Vec2> graph;
    for(size_t i = 0; i < points.size(); ++i) {
        graph.addNode(i, points[i]);
    }
    for(size_t i = 0; i < points.size(); ++i) {
        for(size_t j = 0; j < points.size(); ++j) {
            double distance = euclidean_distance(points[i], points[j]);
            if (distance == 0) {
                continue;
            }
            if (distance < threshold) {
                graph.addEdge(i, j, distance);
            }
        }
    }
    return graph;
}

template<class Key, class Index>
class IndexKeyMapper {
    std::map<Key, Index> keyToIndex;
    std::map<Index, Key> indexToKey;
    void addKeyToIndex(Key key, Index index) {
        keyToIndex[key] = index;
        indexToKey[index] = key;
    }
    Key getKey(Index index) {
        return indexToKey[index];
    }

    Index getIndex(Key key) {
        return keyToIndex[key];
    }
};

using Index = size_t;

struct RawEdge {
    Index dest;
    Weight weight;
};

class RawGraph {
public:
    RawGraph(std::vector<Vec2> nodes): nodes(nodes) {
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

class RawAStar {
    using Index = size_t;
    using Candidate = std::pair<Index, Path<Index>>;
public:
    RawAStar(RawGraph graph): graph(graph) {};

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
    RawGraph graph;

    struct FirstIsLargerWithDistance {
        const RawGraph& graph;
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

RawGraph connectPoints2(const std::vector<Vec2>& points, double threshold) {
    RawGraph graph(points);

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
    Graph<std::string, Vec2> g;
    g.addNode("A", {0, 4});
    g.addNode("B", {4, 4});
    g.addNode("C", {0, 0});
    g.addNode("D", {10, 0});
    g.addNode("E", {0, 1});
    g.addNode("F", {-1, -1});
    g.addEdge("A", "B", 2);
    g.addEdge("E", "A", 3);
    g.addEdge("C", "A", 1);
    g.addEdge("C", "D", 10);
    g.addEdge("C", "F", euclidean_distance({0, 0}, {-1, -1}));
    std::cout << g << std::endl;
    std::cout << g.reprEdges() << std::endl;
    AStar<std::string, Vec2> a(euclidean_distance);
    std::cout << a.shortest_path(g, "C", "B") << std::endl;

    /* auto points = randomPoints(1000, {0, 0}, {100, 100});
    auto graph = connectPoints(points, 5);
    AStar<size_t, Vec2> aStar(euclidean_distance);
    std::cout << aStar.shortest_path(graph, 700, 500) << std::endl;

    for(int j = 0; j < points.size() && j < 10; j++) {
        std::cout << j << " ";
        auto path = aStar.shortest_path(graph, 0, j);
        std::cout << path.nodes.size() << std::endl;
    }*/

    auto points = randomPoints(1000, {0, 0}, {100, 100});
    auto graph = connectPoints2(points, 5);
    RawAStar aStar(graph);
    std::cout << aStar.shortest_path(700, 500) << std::endl;
    for(int j = 0; j < points.size(); j++) {
        std::cout << j << " ";
        auto path = aStar.shortest_path(0, j);
        std::cout << path.nodes.size() << std::endl;
    }

    size_t sum = 0;
    for(int i = 0; i < points.size(); i++) {
        sum += graph.edgesFrom(i).size();
    }
    std::cout << points.size() << " " << sum << std::endl;


    return 0;
}

