#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>

#include "pathing/randomPoints.hpp"
#include "pathing/a_star.hpp"

int main() {
    auto points = pathing::randomPoints(1000, {0, 0}, {100, 100});
    auto graph = pathing::connectPointsWithinThreshold(points, 5);
    pathing::AStar aStar(graph);
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