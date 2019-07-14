from typing import *
from path_search4.graph import Graph
from path_search4.vec2 import Vec2, euclidean_distance
from path_search4.a_star import AStar

starmap = Graph()

class Sun(NamedTuple):
    color: str

class Planet:
    pass

class Fleet:
    pass

class StarSystem(NamedTuple):
    name: str
    position: Vec2
    suns: List[Sun]
    planets: List[Planet]
    fleets: List[Fleet]

    @property
    def owners(self):
        pass

starmap.add_node("Sol", StarSystem(
    "Sol", Vec2(10, 10), [Sun(color="white")], [], []))
starmap.add_node("Deneb", StarSystem(
    "Deneb", Vec2(100, 10), [Sun(color="red")], [], []))
starmap.add_node("Sirius", StarSystem(
    "Sirius", Vec2(10, 100), [Sun(color="blue")], [], []))
starmap.add_node("Alpha Centauri", StarSystem(
    "Alpha Centauri", Vec2(10, 30),
    [Sun(color="red"), Sun(color="red"), Sun(color="red")], [], []))

def add_hyperlane(graph, key1, key2):
    p1 = graph.get_node_data(key1).position
    p2 = graph.get_node_data(key2).position
    distance = euclidean_distance(p1, p2)
    starmap.add_symmetric_edge(key1, key2, distance)

add_hyperlane(starmap, "Sol", "Alpha Centauri")
add_hyperlane(starmap, "Sol", "Sirius")
add_hyperlane(starmap, "Sirius", "Deneb")

pathing = AStar(
    starmap,
    lambda data1, data2: euclidean_distance(data1.position, data2.position))

print(pathing.shortest_path_with_metadata("Alpha Centauri", "Deneb"))
