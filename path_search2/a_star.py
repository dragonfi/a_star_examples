from typing import NamedTuple, MutableMapping, Sequence, Callable, Type, Tuple, Any, List
from collections import OrderedDict

from .graph import Graph, Node, DistanceFn

class Path(NamedTuple):
    weight: float
    path: Sequence[Node]

    @property
    def target(self):
        return self.path[-1]

def a_star(graph: Graph, start: Node, target: Node) -> Path:
    (path, explored, candidates) = a_star_with_metadata(graph, start, target)
    return path

def a_star_with_metadata(graph: Graph, start: Node, target: Node) -> Tuple[Path, Any, Any]:
    print(graph.nodes[:1])

    def candidate_sorting_key(candidate: Tuple[Node, Path]) -> float:
        node, path = candidate
        return path.weight + graph.distance_fn(node, target)

    if start == target:
        return (Path(0, [start]), [], [])

    explored: Any = {}
    candidates: Any = OrderedDict(
        {edge.dest: Path(edge.weight, [start, edge.dest]) for edge in start.edges})
    print(explored, candidates)

    while candidates:
        candidates = OrderedDict(sorted(candidates.items(), key = candidate_sorting_key))
        node, path = candidates.popitem(last=False)
        if node == target:
            print(path.path[:1])
            return (path, explored, candidates)
        if node in explored.keys() and explored[node].weight <= path.weight:
            pass
        else:
            explored[node] = path

        new_candidates = {
            edge.dest: Path(path.weight + edge.weight, path.path + [edge.dest])
            for edge in node.edges
            if edge.dest not in explored.keys()}

        for key, value in new_candidates.items():
            if key in candidates.keys() and candidates[key].weight <= value.weight:
                pass
            else:
                candidates[key] = value

    return (None, explored, candidates)
