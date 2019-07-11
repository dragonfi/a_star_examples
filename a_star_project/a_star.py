from collections import namedtuple, OrderedDict
from math import sqrt

from .vec2 import Vec2

Path = namedtuple("Path", "weight path")

def a_star(graph, start, target, heuristic = lambda _: 0):
    if start == target:
        return Path(start, 0, [start])

    explored = {start: Path(0, [start])}
    candidates = {edge.node: Path(edge.weight, [start, edge.node]) for edge in graph[start]}

    while candidates:
        candidates = OrderedDict(sorted(candidates.items(), key=lambda item: item[1].weight + heuristic(item[0])))
        node, path = candidates.popitem(last=False)
        if node == target:
            print(len(explored), len(candidates))
            return path
        if node in explored.keys() and explored[node].weight <= path.weight:
            pass
        else:
            explored[node] = path

        new_candidates = {
            edge.node: Path(path.weight + edge.weight, path.path + [edge.node])
            for edge in graph[node]
            if edge.node not in explored.keys()}

        for key, value in new_candidates.items():
            if key in candidates.keys() and candidates[key].weight <= value.weight:
                pass
            else:
                candidates[key] = value
    print(len(explored), len(candidates))
    return None
