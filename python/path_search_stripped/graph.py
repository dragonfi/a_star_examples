from collections import namedtuple

Edge = namedtuple("Edge", "source dest weight")
Node = namedtuple("Node", "key data")

class Graph():
    """
    Node data should be hashable and unique
    """
    def __init__(self)        :
        self._nodes                               = {}
        self._edges                                                = {}

    def add_node(self, key        , data      ):
        if key in self._nodes.keys():
            raise ValueError("Already contains this node")
        self._nodes[key] = Node(key, data)
        self._edges[key] = []

    def add_edge(self, key1        , key2        , weight       )        :
        edge = Edge(key1, key2, weight)
        self._edges[key1].append(edge)

    def edges_from(self, key        )                  :
        return self._edges[key]

    def get_node_data(self, key        )        :
        return self._nodes[key].data

    def add_symmetric_edge(self, key1        , key2        , weight       )        :
        self.add_edge(key1, key2, weight)
        self.add_edge(key2, key1, weight)

    @property
    def nodes(self)                  :
        return list(self._nodes.values())

    @property
    def edges(self)                  :
        return [edge for edges in self._edges.values() for edge in edges]
