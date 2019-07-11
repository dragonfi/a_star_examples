import random
import typing
from collections import namedtuple

from .vec2 import Vec2

Edge = namedtuple("Edge", "node weight")

def random_euclidean_points(n=1000, min=Vec2(0, 0) , max=Vec2(100, 100)):
    import random
    xs = [random.uniform(min.x, max.x) for _ in range(n)]
    ys = [random.uniform(min.y, max.y) for _ in range(n)]
    return [Vec2(xs[i], ys[i]) for i in range(n)]

def connect_points(points: typing.List[Vec2], max_distance=10) -> typing.Dict[int, typing.List[Edge]]:
    graph = {}
    for i, point in enumerate(points):
        edges = []
        for j, other_point in enumerate(points):
            distance = (point - other_point).abs()
            if j != i and distance <= max_distance:
                edges.append(Edge(j, distance))
        graph[i] = edges
    return graph
