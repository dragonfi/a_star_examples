from .points import random_euclidean_points, connect_points
from .a_star import a_star

def run_all():
    example1()

def example1():
    print("generating points...")
    points = random_euclidean_points(1000)
    print(len(points), points[0:5])

    print("generating connections...")
    graph = connect_points(points, 10)

    _disconnected = [key for (key, value) in graph.items() if len(value) == 0]
    print("disconnected:", _disconnected)

    print("calculating with heuristic 0...")
    heuristic = lambda node: (points[node] - points[100]).abs()
    path = a_star(graph, 0, 100, lambda node: 0)
    print(path)

    print("calculating with heuristic euclidean distance...")
    heuristic = lambda node: (points[node] - points[100]).abs()
    path2 = a_star(graph, 0, 100, heuristic)
    print(path2)
    assert(path == path2)
