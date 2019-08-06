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

        StaticVAO(const StaticVAO& other) = delete;
        StaticVAO(StaticVAO&& other); // implemented
        StaticVAO& operator=(const StaticVAO&) = delete;
        StaticVAO& operator=(StaticVAO&&) = delete;

        ~StaticVAO();

        void draw(GLuint shape) const;
    private:
        void set_points(const std::vector<Point>& points);
        void set_colors(const std::vector<Color>& colors);
        GLuint vbo[2] = {0, 0}, vao[1] = {0};
        GLuint m_shape;
        GLsizei vertex_count = 0;
        static const int position_attribute_index = 0;
        static const int color_attribute_index = 1;
    };
}