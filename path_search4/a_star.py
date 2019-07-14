from typing import *
from collections import OrderedDict

from .graph import Graph

Data = TypeVar("Data")
NodeId = Hashable

class Path(NamedTuple):
    weight: float
    nodes: List[NodeId]

    @property
    def dest(self):
        return self.nodes[-1]

    @property
    def source(self):
        return self.nodes[0]


class AStarResult(NamedTuple):
    path: Path
    explored: Mapping[NodeId, Path]
    candidates: Mapping[NodeId, Path]

class AStar(Generic[Data]):
    def __init__(self, graph: Graph[Data], heuristic: Callable[[Data, Data], float]) -> None:
        self._graph = graph
        self._heuristic = heuristic

    def shortest_path(self, source: NodeId, dest: NodeId) -> Path:
        return self.shortest_path_with_metadata(source, dest).path

    def shortest_path_with_metadata(self, source: NodeId, dest: NodeId) -> AStarResult:
        def candidate_sorting_key(candidate: Tuple[NodeId, Path]) -> float:
            node, path = candidate
            node_data = self._graph.get_node_data(node)
            dest_data = self._graph.get_node_data(dest)
            return path.weight + self._heuristic(node_data, dest_data)

        if source == dest:
            return AStarResult(Path(0, [source]), {}, {})

        explored: Dict[NodeId, Path] = {}

        source_edges = self._graph.edges_from(source)
        candidates: Dict[NodeId, Path] = OrderedDict(
            {source: Path(edge.weight, [source, edge.dest]) for edge in source_edges}
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
