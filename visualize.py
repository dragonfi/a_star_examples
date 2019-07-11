import pyglet

from path_search.vec2 import Vec2
from path_search.a_star import a_star_with_metadata

window = pyglet.window.Window()

label = pyglet.text.Label('Hello, world',
                          font_name='Times New Roman',
                          font_size=36,
                          x=window.width//2, y=window.height//2,
                          anchor_x='center', anchor_y='center')

def point_to_opengl(point, r = 2):
    points = [
        point.x - r, point.y - r,
        point.x - r, point.y + r,
        point.x + r, point.y - r,
        point.x + r, point.y + r,
    ]

    indexes = [0, 1, 2, 1, 2, 3]
    return (indexes, points)

def draw_nodes(points, color = (128, 128, 128, 128)):
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


def draw_edges(points, graph):
    vertices = []
    for point in points:
        vertices += [point.x, point.y]

    index = []
    for node, edges in graph.items():
        for edge in edges:
            index += [node, edge.node]

    print(len(vertices), len(index))

    grey = [128, 128, 128, 128]
    colors = grey * len(points)

    pyglet.graphics.draw_indexed(
        len(vertices) // 2, pyglet.gl.GL_LINES,
        index, ('v2f', vertices), ('c4B', colors))

def duplicate_middle(seq):
    result = [seq[0]]
    for item in seq[1:-1]:
        result.append(item)
        result.append(item)
    result.append(seq[-1])
    print(result)
    return result

def draw_path(points, path):
    vertices = []
    for point in points:
        vertices += [point.x, point.y]

    orange = [255, 255, 0, 0]
    colors = orange * len(points)

    index = duplicate_middle(path.path)

    pyglet.graphics.draw_indexed(
        len(vertices) // 2, pyglet.gl.GL_LINES,
        index, ('v2f', vertices), ('c4B', colors))

    highlighted_points = [point for (index, point) in enumerate(points) if index in path.path]
    draw_nodes(highlighted_points, color = orange)


from path_search.points import random_euclidean_points, connect_points

def generate_graph():
    points = random_euclidean_points(1000, Vec2(50, 50), Vec2(590, 430))
    graph = connect_points(points, 20)
    return points, graph

def find_path(heuristic):
    return a_star_with_metadata(GRAPH, START, STOP, heuristic)


POINTS, GRAPH = generate_graph()
START, STOP = 100, 200

HEURISTIC = lambda node: (POINTS[node] - POINTS[STOP]).abs()
USING_HEURISTIC = False

PATH, EXPLORED, CANDIDATES = find_path(lambda _: 0)


@window.event
def on_draw():
    window.clear()
    #label.draw()

    # pyglet.graphics.draw(
    #     2, pyglet.gl.GL_POINTS,
    #     ('v2f', (10.0, 15.0, 30.0, 35.0)))
    #
    # pyglet.graphics.draw_indexed(
    #     2, pyglet.gl.GL_POINTS,
    #     [0, 1],
    #     ('v2i', (10, 15, 30, 35)))
    #
    # pyglet.graphics.draw_indexed(4, pyglet.gl.GL_TRIANGLES,
    #     [0, 1, 2, 0, 2, 3],
    #     ('v2i', (100, 100,
    #              150, 100,
    #              150, 150,
    #              100, 150))
    #
    # pyglet.graphics.draw_indexed(
    #     3, pyglet.gl.GL_TRIANGLES,
    #     [0, 1, 2], ('v2f', [10, 10, 20, 20, 10, 20]))

    draw_nodes(POINTS)
    draw_edges(POINTS, GRAPH)

    #draw_nodes([point for (index, point) in enumerate(POINTS) if index in EXPLORED2], color = (128, 0, 128, 0))
    #draw_nodes([point for (index, point) in enumerate(POINTS) if index in CANDIDATES2], color = (0, 128, 128, 0))

    #if PATH2:
    #    draw_path(POINTS, PATH2)

    draw_nodes([point for (index, point) in enumerate(POINTS) if index in EXPLORED], color = (128, 0, 128, 0))
    draw_nodes([point for (index, point) in enumerate(POINTS) if index in CANDIDATES], color = (0, 128, 128, 0))
    if PATH:
        draw_path(POINTS, PATH)

    draw_nodes([POINTS[START]], color = (0, 255, 0, 0))
    draw_nodes([POINTS[STOP]], color = (255, 0, 0, 0))

from pyglet.window import key

def generate_new_graph():
    global POINTS, GRAPH
    POINTS, GRAPH = generate_graph()

def change_heuristic():
    global PATH, EXPLORED, CANDIDATES, USING_HEURISTIC
    if USING_HEURISTIC:
        USING_HEURISTIC = False
        PATH, EXPLORED, CANDIDATES = find_path(lambda _: 0)
    else:
        USING_HEURISTIC = True
        PATH, EXPLORED, CANDIDATES = find_path(heuristic=HEURISTIC)

@window.event
def on_key_press(symbol, modifiers):
    global USING_HEURISTIC
    if symbol == key.SPACE:
        change_heuristic()
    if symbol == key.R:
        generate_new_graph()
        USING_HEURISTIC = not USING_HEURISTIC
        change_heuristic()


    print(len(EXPLORED), len(CANDIDATES), PATH, USING_HEURISTIC)


pyglet.app.run()
