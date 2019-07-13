
from .vec2 import Vec2, euclidean_distance
from .graph import Edge, Node, Graph
from typing import Sequence, Mapping, NamedTuple, Tuple

def random_euclidean_points(n=1000, min=Vec2(0, 0) , max=Vec2(100, 100)) -> Sequence[Vec2]:
    import random
    xs = [random.uniform(min.x, max.x) for _ in range(n)]
    ys = [random.uniform(min.y, max.y) for _ in range(n)]
    return [Vec2(xs[i], ys[i]) for i in range(n)]

def connect_points(points: Sequence[Vec2], max_distance=10) -> Graph:
    nodes = [Node(point, []) for point in points]
    for node in nodes:
        for other_node in nodes:
            distance = euclidean_distance(node.data, other_node.data)
            if node != other_node and distance <= max_distance:
                node.edges.append(Edge(node, distance, other_node))

    return Graph(nodes, lambda node, other: euclidean_distance(node.data, other.data))
