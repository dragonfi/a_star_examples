#include <GL/glew.h>
#include <vector>
#include <stdexcept>

#include "vao.hpp"
#include "types.hpp"

namespace graphics {
    StaticVAO::StaticVAO(
        GLuint shape,
        const std::vector<Point>& points,
        const std::vector<Color>& colors)
        : m_shape(shape)
    {
        glGenVertexArrays(1, vao);
        glGenBuffers(2, vbo);
        set_points(points);
        set_colors(colors);
    }

    void StaticVAO::draw() {
        draw(m_shape);
    }

    void StaticVAO::draw(GLuint shape) {
        glBindVertexArray(vao[0]);

        glEnableVertexAttribArray(position_attribute_index);
        glEnableVertexAttribArray(color_attribute_index);

        glDrawArrays(shape, 0, vertex_count);
    }

    void StaticVAO::set_points(const std::vector<Point>& points) {
        glBindVertexArray(vao[0]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

        glBufferData(
            GL_ARRAY_BUFFER, points.size() * Point::mem_size,
            points.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(position_attribute_index, Point::length, GL_FLOAT, GL_FALSE, 0, 0);

        vertex_count = points.size();

    }

    void StaticVAO::set_colors(const std::vector<Color>& colors) {
        glBindVertexArray(vao[0]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

        glBufferData(
            GL_ARRAY_BUFFER, colors.size() * Color::mem_size,
            colors.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(color_attribute_index, Color::length, GL_FLOAT, GL_FALSE, 0, 0);

        if (static_cast<size_t>(vertex_count) != colors.size()) {
            throw std::runtime_error("Vertex count mismatch");
        }

    }
}