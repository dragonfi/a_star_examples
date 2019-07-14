from unittest import TestCase

from .graph import Graph


class TestGraph(TestCase):
    def test_nodes_can_be_added_and_data_retrieved(self):
        graph = Graph()
        graph.add_node("Sol", "sol")
        graph.add_node("Sirius", "sirius")

        self.assertEqual(graph.get_node_data("Sol"), "sol")
        self.assertEqual(graph.get_node_data("Sirius"), "sirius")

        self.assertEqual(len(graph.nodes), 2)

    def test_edges_can_be_added(self):
        graph = Graph()
        graph.add_node("Sol", "sol")
        graph.add_node("Sirius", "sirius")

        graph.add_edge("Sol", "Sirius", 10)

        self.assertEqual(len(graph.edges_from("Sol")), 1)
        self.assertEqual(graph.edges_from("Sol")[0].dest, "Sirius")
        self.assertEqual(graph.edges_from("Sol")[0].weight, 10)

        self.assertEqual(len(graph.edges), 1)
