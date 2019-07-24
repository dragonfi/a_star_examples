from path_search.vec2 import Vec2, euclidean_distance
from path_search.a_star import AStar
from path_search.points import random_euclidean_points, connect_points
from path_search.graph import Graph

points = random_euclidean_points(1000, Vec2(0, 0), Vec2(100, 100))
graph = connect_points(points, 5)
start = points[100]
end = points[200]

pathing = AStar(graph, euclidean_distance)

for i in range(1000):
    print(i, end=" ")
    path, explored, candidates = pathing.shortest_path_with_metadata(start, points[i])
    print(len(path.nodes) if path else 0)
