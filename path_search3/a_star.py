from typing import *
from collections import OrderedDict

from .graph import Graph, _Node

Data = Any

class Path(NamedTuple):
    weight: float
    nodes: Sequence

    @property
    def dest(self):
        return self.nodes[-1]

    @property
    def source(self):
        return self.nodes[0]


class AStarResult(NamedTuple):
    path: Path
    explored: Sequence[_Node]
    candidates: Sequence[_Node]

class AStar:
    def __init__(self, graph: Graph[Data], heuristic) -> None:
        self._graph = graph
        self._heuristic = heuristic

    def shortest_path(self, source: Data, dest: Data) -> Path:
        return self.shortest_path_with_metadata(source, dest).path

    def shortest_path_with_metadata(self, source: Data, dest: Data) -> AStarResult:
        def candidate_sorting_key(candidate: Tuple[Data, Path]) -> float:
            node, path = candidate
            return path.weight + self._heuristic(node, dest)

        if source == dest:
            return AStarResult(Path(0, [source]), [], [])

        explored: Any = {}

        source_edges = self._graph.edges_from(source)
        candidates: Any = OrderedDict(
            {edge.dest: Path(edge.weight, [source, edge.dest]) for edge in source_edges}
        )

        while candidates:
            candidates = OrderedDict(sorted(candidates.items(), key = candidate_sorting_key))
            node, path = candidates.popitem(last=False)
            if node == dest:
                return AStarResult(path, explored, candidates)
            if node in explored.keys() and explored[node].weight <= path.weight:
                pass
            else:
                explored[node] = path
            new_candidates = {
                edge.dest: Path(path.weight + edge.weight, path.nodes + [edge.dest])
                for edge in self._graph.edges_from(node)
                if edge.dest not in explored.keys()}

            for key, value in new_candidates.items():
                if key in candidates.keys() and candidates[key].weight <= value.weight:
                    pass
                else:
                    candidates[key] = value
        return AStarResult(None, explored, candidates)
