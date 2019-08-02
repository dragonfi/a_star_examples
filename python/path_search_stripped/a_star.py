from collections import OrderedDict, namedtuple

from .graph import Graph


class Path():
    def __init__(self, weight, nodes):
        self.weight = weight
        self.nodes = nodes

    @property
    def dest(self):
        return self.nodes[-1]

    @property
    def source(self):
        return self.nodes[0]


AStarResult = namedtuple("AStarResult", "path explored candidates")


class AStar():
    def __init__(self, graph             , heuristic                               )        :
        self._graph = graph
        self._heuristic = heuristic

    def shortest_path(self, source        , dest        )        :
        return self.shortest_path_with_metadata(source, dest).path

    def shortest_path_with_metadata(self, source        , dest        )               :
        def candidate_sorting_key(candidate                     )         :
            node, path = candidate
            node_data = self._graph.get_node_data(node)
            dest_data = self._graph.get_node_data(dest)
            return path.weight + self._heuristic(node_data, dest_data)

        explored                     = {}
        candidates                     = OrderedDict({source: Path(0, [source])})

        while candidates:
            candidates = OrderedDict(sorted(candidates.items(), key=candidate_sorting_key))
            node, path = candidates.popitem(last=False)
            if node == dest:
                return AStarResult(path, explored, candidates)
            if node not in explored.keys() or explored[node].weight > path.weight:
                explored[node] = path

            new_candidates = {
                edge.dest: Path(path.weight + edge.weight, path.nodes + [edge.dest])
                for edge in self._graph.edges_from(node)
                if edge.dest not in explored.keys()}

            for key, value in new_candidates.items():
                if key not in candidates.keys() or candidates[key].weight > value.weight:
                    candidates[key] = value
        return AStarResult(None, explored, candidates)
