from typing import *

Data = TypeVar('Data')

class _Node:
    def __init__(self, data: Any) -> None:
        self.data = data

class Edge(NamedTuple):
    source: Hashable
    dest: Hashable
    weight: float

class Node(NamedTuple):
    key: Hashable
    data: Any

class Graph(Generic[Data]):
    """
    Node data should be hashable and unique
    """
    def __init__(self) -> None:
        self._nodes: MutableMapping[Hashable, Node] = {}
        self._edges: MutableMapping[Hashable, MutableSequence[Edge]] = {}

    def add_node(self, key: Hashable, data: Data):
        if key in self._nodes.keys():
            raise ValueError("Already contains this node")
        self._nodes[key] = Node(key, data)
        self._edges[key] = []

    def add_edge(self, key1: Hashable, key2: Hashable, weight: float) -> None:
        edge = Edge(key1, key2, weight)
        self._edges[key1].append(edge)

    def edges_from(self, key):
        return self._edges[key]

    def get_node_data(self, key):
        return self._nodes[key].data

    def add_symmetric_edge(self, key1, key2, weight):
        self.add_edge(key1, key2, weight)
        self.add_edge(key2, key1, weight)

    @property
    def nodes(self):
        return self._nodes.values()

    @property
    def edges(self):
        return [edge for edges in self._edges.values() for edge in edges]
