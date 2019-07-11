import pyglet

from path_search.vec2 import Vec2

window = pyglet.window.Window()

label = pyglet.text.Label('Hello, world',
                          font_name='Times New Roman',
                          font_size=36,
                          x=window.width//2, y=window.height//2,
                          anchor_x='center', anchor_y='center')

def point_to_opengl(point):
    r = 5
    points = [
        point.x - r, point.y - r,
        point.x - r, point.y + r,
        point.x + r, point.y - r,
        point.x + r, point.y + r,
    ]
    indexes = [0, 1, 2, 1, 2, 3]
    return (indexes, points)

def draw_points(points):
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


@window.event
def on_draw():
    window.clear()
    label.draw()

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

    draw_points([Vec2(10, 10), Vec2(20, 20), Vec2(30, 30)])

    # pyglet.graphics.draw_indexed(
    #     3, pyglet.gl.GL_TRIANGLES,
    #     [0, 1, 2], ('v2f', [10, 10, 20, 20, 10, 20]))

pyglet.app.run()
