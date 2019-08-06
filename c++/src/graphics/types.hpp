#pragma once

#include <GL/glew.h>
#include <vector>

namespace graphics {
    struct Color {
        GLfloat r, g, b, a;
        static const GLsizei length = 4;
        static const GLsizei mem_size = length * sizeof(GLfloat);
    };

    struct Point {
        GLfloat x, y, z;
        static const GLsizei length = 3;
        static const GLsizei mem_size = length * sizeof(GLfloat);
    };

    class Renderer;
    class ShaderProgram;
    class StaticVAO;

    struct Scene {
        Renderer& renderer;
        ShaderProgram& shader;
        std::vector<StaticVAO>& vaos;
    };
}