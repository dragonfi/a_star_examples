from unittest import TestCase

from path_search.graph import Graph
from path_search.vec2 import Vec2, euclidean_distance
from path_search.a_star import AStar


class StarSystem(NamedTuple):
    #ame: str
    #osition: Vec2
    data      = {}

    @property
    def owners(self):
        pass


def add_hyperlane(graph, key1, key2):
    p1 = graph.get_node_data(key1).position
    p2 = graph.get_node_data(key2).position
    distance = euclidean_distance(p1, p2)
    graph.add_edge(key1, key2, distance)
    graph.add_edge(key2, key1, distance)


class TestAStar(TestCase):
    def test_starmap_test_case(self):
        starmap = Graph()

        starmap.add_node("Sol", StarSystem("Sol", Vec2(10, 10)))
        starmap.add_node("Deneb", StarSystem("Deneb", Vec2(100, 10)))
        starmap.add_node("Sirius", StarSystem("Sirius", Vec2(10, 100)))
        starmap.add_node("Alpha Centauri", StarSystem("Alpha Centauri", Vec2(10, 30)))

        add_hyperlane(starmap, "Sol", "Alpha Centauri")
        add_hyperlane(starmap, "Sol", "Sirius")
        add_hyperlane(starmap, "Sirius", "Deneb")

        pathing = AStar(
            starmap,
            lambda data1, data2: euclidean_distance(data1.position, data2.position))

        ac_to_deneb = pathing.shortest_path_with_metadata("Alpha Centauri", "Deneb")
        sol_to_sol = pathing.shortest_path_with_metadata("Sol", "Sol")

        self.assertEqual(
            ac_to_deneb.path.nodes,
            ["Alpha Centauri", "Sol", "Sirius", "Deneb"])
