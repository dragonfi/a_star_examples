from typing import *

"""
S: Single Responsibility
O: Open Closed
L: Liskov Substitution
I: Interface Segregation
D: Dependency Inversion
"""

Data = TypeVar('Data')

class _Node:
    def __init__(self, data: Any) -> None:
        self.data = data

class Edge(NamedTuple):
    source: Hashable
    dest: Hashable
    weight: float

class Graph(Generic[Data]):
    """
    Node data should be hashable and unique
    """
    def __init__(self, key_fn: Callable[[Data], Hashable] = lambda data: data) -> None:
        self._nodes: MutableMapping[Hashable, Data] = {}
        self._edges: MutableMapping[Hashable, MutableSequence[Edge]] = {}
        self._key_fn = key_fn

    def add_node(self, data: Data):
        if self._key_fn(data) in self._nodes.keys():
            raise ValueError("Already contains this node")
        self._nodes[self._key_fn(data)] = data
        self._edges[self._key_fn(data)] = []

    def add_edge(self, data1: Data, data2: Data, weight: float) -> None:
        key1 = self._key_fn(data1)
        key2 = self._key_fn(data2)
        edge = Edge(key1, key2, weight)
        self._edges[key1].append(edge)

    def edges_from(self, data):
        return self._edges[self._key_fn(data)]

    @property
    def nodes(self):
        return [data for data in self._nodes.values()]

    @property
    def edges(self):
        return [edge for edges in self._edges.values() for edge in edges]
