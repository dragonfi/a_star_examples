#pragma once

#include <GL/glew.h>

#include "types.hpp"

namespace graphics {
    class StaticVAO {
    public:
        StaticVAO(
            GLuint shape,
            const std::vector<Point>& points,
            const std::vector<Color>& colors);
        void draw();

        void draw(GLuint shape);
    private:
        void set_points(const std::vector<Point>& points);
        void set_colors(const std::vector<Color>& colors);
        GLuint vbo[2], vao[1];
        GLuint m_shape;
        GLsizei vertex_count = 0;
        static const int position_attribute_index = 0;
        static const int color_attribute_index = 1;
    };
}