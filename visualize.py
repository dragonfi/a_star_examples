import pyglet

from path_search.vec2 import Vec2

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

def draw_nodes(points):
    vertices = []
    index = []
    start_index = 0
    for point in points:
        new_index, vs = point_to_opengl(point)
        vertices += vs
        index += [i + start_index for i in new_index]
        start_index += len(vs) // 2

    pyglet.graphics.draw_indexed(
        len(vertices) // 2, pyglet.gl.GL_TRIANGLES,
        index, ('v2f', vertices))


def draw_edges(points, graph):
    vertices = []
    for point in points:
        vertices += [point.x, point.y]

    index = []
    for node, edges in graph.items():
        for edge in edges:
            index += [node, edge.node]

    print(len(vertices), len(index))

    pyglet.graphics.draw_indexed(
        len(vertices) // 2, pyglet.gl.GL_LINES,
        index, ('v2f', vertices))

from path_search.points import random_euclidean_points, connect_points

POINTS = random_euclidean_points(1000, Vec2(50, 50), Vec2(590, 430))
GRAPH = connect_points(POINTS, 20)

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

pyglet.app.run()
