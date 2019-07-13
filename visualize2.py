import pyglet

from path_search2.vec2 import Vec2, euclidean_distance
from path_search2.a_star import a_star_with_metadata
from path_search2.points import random_euclidean_points, connect_points
from path_search2.graph import Graph

window = pyglet.window.Window(width=800, height=600, resizable=True)

label = pyglet.text.Label('Hello, world',
                          font_name='Times New Roman',
                          font_size=36,
                          x=window.width//2, y=window.height//2,
                          anchor_x='center', anchor_y='center')

def point_to_opengl(point):
    r = POINT_SIZE
    points = [
        point.x - r, point.y - r,
        point.x - r, point.y + r,
        point.x + r, point.y - r,
        point.x + r, point.y + r,
    ]

    indexes = [0, 1, 2, 1, 2, 3]
    return (indexes, points)

def draw_nodes(nodes, color = (128, 128, 128, 128)):
    points = [node.data for node in nodes]
    vertices = []
    index = []
    start_index = 0
    for point in points:
        new_index, vs = point_to_opengl(point)
        vertices += vs
        index += [i + start_index for i in new_index]
        start_index += len(vs) // 2

    colors = list(color) * (len(vertices) // 2)

    pyglet.graphics.draw_indexed(
        len(vertices) // 2, pyglet.gl.GL_TRIANGLES,
        index, ('v2f', vertices), ('c4B', colors))


def draw_edges(edges):

    vertices = []
    for edge in edges:
        source = edge.source.data
        dest = edge.dest.data
        vertices += [source.x, source.y, dest.x, dest.y]

    grey = [128, 128, 128, 128]
    size = len(vertices) // 2
    colors = grey * size

    pyglet.graphics.draw(
        size, pyglet.gl.GL_LINES,
        ('v2f', vertices), ('c4B', colors))

def duplicate_middle(seq):
    result = [seq[0]]
    for item in seq[1:-1]:
        result.append(item)
        result.append(item)
    result.append(seq[-1])
    return result

def draw_path(path):
    vertices = []
    points = duplicate_middle([node.data for node in path.path])

    for point in points:
        vertices += [point.x, point.y]

    orange = [255, 255, 0, 0]
    colors = orange * len(points)

    pyglet.graphics.draw(
        len(vertices) // 2, pyglet.gl.GL_LINES,
        ('v2f', vertices), ('c4B', colors))

    draw_nodes(path.path, color = orange)


def generate_graph():
    global POINT_SIZE
    w = window.width
    h = window.height
    size = min(w, h)
    threshold = 0.036 * size
    POINT_SIZE = 0.005 * size

    points = random_euclidean_points(1000, Vec2(50, 50), Vec2(size - 50, size - 50))
    graph = connect_points(points, threshold)
    return graph, graph.nodes[100], graph.nodes[200]

def find_path():
    return a_star_with_metadata(GRAPH, START, STOP)

GRAPH, START, STOP = generate_graph()

POINT_SIZE = 2

USING_HEURISTIC = True

PATH, EXPLORED, CANDIDATES = find_path()


@window.event
def on_draw():
    window.clear()

    draw_nodes([node for node in GRAPH.nodes])
    draw_edges([edge for edge in GRAPH.edges])

    draw_nodes([node for node in EXPLORED], color = (128, 0, 128, 0))
    draw_nodes([node for node in CANDIDATES], color = (0, 128, 128, 0))
    if PATH:
        draw_path(PATH)

    draw_nodes([START], color = (255, 0, 0, 0))
    draw_nodes([STOP], color = (0, 255, 0, 0))

@window.event
def on_resize(width, height):
    global PATH, EXPLORED, CANDIDATES
    generate_new_graph()
    PATH, EXPLORED, CANDIDATES = find_path()

from pyglet.window import key

def generate_new_graph():
    global GRAPH, START, STOP
    GRAPH, START, STOP = generate_graph()

def change_heuristic():
    global GRAPH, START, STOP, USING_HEURISTIC
    if USING_HEURISTIC:
        GRAPH = Graph(GRAPH.nodes, lambda _, __: 0)
    else:
        GRAPH = Graph(
            GRAPH.nodes,
            lambda node, other: euclidean_distance(node.data, other.data))
    USING_HEURISTIC = not USING_HEURISTIC

@window.event
def on_key_press(symbol, modifiers):
    global PATH, EXPLORED, CANDIDATES
    if symbol == key.SPACE:
        change_heuristic()
        PATH, EXPLORED, CANDIDATES = find_path()
    if symbol == key.R:
        generate_new_graph()
        PATH, EXPLORED, CANDIDATES = find_path()
    if symbol == key.ESCAPE:
        pyglet.app.exit()


pyglet.app.run()
