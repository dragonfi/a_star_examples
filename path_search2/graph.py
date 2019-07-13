from typing import NamedTuple, Any, Sequence, MutableSequence, Callable
from collections import namedtuple
from math import sqrt

from .vec2 import Vec2

# TODO(python3.7): edges: List[Edge], but mypy and python3.6 does not support it yet
class Node(NamedTuple):
    data: Any
    edges: MutableSequence[Any]

    def __hash__(self):
        return hash(self.data)

    def __eq__(self, other):
        return self.data == other.data

    def __repr__(self):
        return f"Node({self.data})"

class Edge(NamedTuple):
    source: Node
    weight: float
    dest: Node

DistanceFn = Callable[[Node, Node], float]

class Graph(NamedTuple):
    nodes: MutableSequence[Node]
    distance_fn: DistanceFn

    @property
    def edges(self):
        return [edge for node in self.nodes for edge in node.edges]
