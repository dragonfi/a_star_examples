import pyglet

from path_search.vec2 import Vec2, euclidean_distance
from path_search.a_star import AStar
from path_search.points import random_euclidean_points, connect_points
from path_search.graph import Graph

window = pyglet.window.Window(width=800, height=600, resizable=True)

label = pyglet.text.Label('Hello, world',
                          font_name='Times New Roman',
                          font_size=36,
                          x=window.width//2, y=window.height//2,
                          anchor_x='center', anchor_y='center')


class GlobalState:
    def __init__(self, w):
        self.heuristic = euclidean_distance
        self.on_resize(w.width, w.height)

    def on_resize(self, width, height):
        self.generate_graph(width, height)
        self.find_path()

    def generate_graph(self, width, height):
        w = window.width
        h = window.height
        size = min(w, h)
        threshold = 0.036 * size
        self.point_size = 0.005 * size

        self.points = random_euclidean_points(1000, Vec2(50, 50), Vec2(size - 50, size - 50))
        self.graph = connect_points(self.points, threshold)
        self.start = self.points[100]
        self.end = self.points[200]

    def find_path(self):
        pathing = AStar(self.graph, self.heuristic)
        path, explored, candidates = pathing.shortest_path_with_metadata(self.start, self.end)
        self.path = path
        self.explored = explored
        self.candidates = candidates

    def on_draw(self):
        self.draw_nodes([node.data for node in self.graph.nodes])
        self.draw_edges([edge for edge in self.graph.edges])

        self.draw_nodes([node for node in self.explored], color = (128, 0, 128, 0))
        self.draw_nodes([node for node in self.candidates], color = (0, 128, 128, 0))
        if self.path:
            self.draw_path(self.path)

        self.draw_nodes([self.start], color = (255, 0, 0, 0))
        self.draw_nodes([self.end], color = (0, 255, 0, 0))

    def point_to_opengl(self, point):
        r = self.point_size
        points = [
            point.x - r, point.y - r,
            point.x - r, point.y + r,
            point.x + r, point.y - r,
            point.x + r, point.y + r,
        ]

        indexes = [0, 1, 2, 1, 2, 3]
        return (indexes, points)

    def draw_nodes(self, points, color = (128, 128, 128, 128)):
        vertices = []
        index = []
        start_index = 0
        for point in points:
            new_index, vs = self.point_to_opengl(point)
            vertices += vs
            index += [i + start_index for i in new_index]
            start_index += len(vs) // 2

        colors = list(color) * (len(vertices) // 2)

        pyglet.graphics.draw_indexed(
            len(vertices) // 2, pyglet.gl.GL_TRIANGLES,
            index, ('v2f', vertices), ('c4B', colors))

    def draw_edges(self, edges):
        vertices = []
        for edge in edges:
            source = edge.source
            dest = edge.dest
            vertices += [source.x, source.y, dest.x, dest.y]

        grey = [128, 128, 128, 128]
        size = len(vertices) // 2
        colors = grey * size

        pyglet.graphics.draw(
            size, pyglet.gl.GL_LINES,
            ('v2f', vertices), ('c4B', colors))

    def duplicate_middle(self, seq):
        result = [seq[0]]
        for item in seq[1:-1]:
            result.append(item)
            result.append(item)
        result.append(seq[-1])
        return result

    def draw_path(self, path):
        vertices = []
        points = self.duplicate_middle([node for node in path.nodes])

        for point in points:
            vertices += [point.x, point.y]

        orange = [255, 255, 0, 0]
        colors = orange * len(points)

        pyglet.graphics.draw(
            len(vertices) // 2, pyglet.gl.GL_LINES,
            ('v2f', vertices), ('c4B', colors))

        self.draw_nodes([point for point in path.nodes], color = orange)

    def change_heuristic(self):
        if self.heuristic == euclidean_distance:
            self.heuristic = lambda _, __: 0
        else:
            self.heuristic = euclidean_distance

        self.find_path()

global_state = GlobalState(window)

@window.event
def on_draw():
    window.clear()
    global_state.on_draw()

@window.event
def on_resize(width, height):
    global_state.on_resize(width, height)

from pyglet.window import key

@window.event
def on_key_press(symbol, modifiers):
    global PATH, EXPLORED, CANDIDATES
    if symbol == key.SPACE:
        global_state.change_heuristic()
    if symbol == key.R:
        global_state.on_resize(window.width, window.height)
    if symbol == key.ESCAPE:
        pyglet.app.exit()

pyglet.app.run()
