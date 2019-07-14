
from .vec2 import Vec2, euclidean_distance
from .graph import Graph
from typing import Sequence, Mapping, NamedTuple, Tuple

def random_euclidean_points(n=1000, min=Vec2(0, 0) , max=Vec2(100, 100)) -> Sequence[Vec2]:
    import random
    xs = [random.uniform(min.x, max.x) for _ in range(n)]
    ys = [random.uniform(min.y, max.y) for _ in range(n)]
    return [Vec2(xs[i], ys[i]) for i in range(n)]

def connect_points(points: Sequence[Vec2], max_distance=10) -> Graph:
    graph: Graph = Graph()
    for point in points:
        graph.add_node(point, point)

    for point in points:
        for other_point in points:
            distance = euclidean_distance(point, other_point)
            if point != other_point and distance <= max_distance:
                graph.add_edge(point, other_point, distance)

    return graph
