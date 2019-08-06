
#include <iostream>
#include <stddef.h>
#include <vector>
#include <exception>

#include <SDL2/SDL.h>

#include "pathing/a_star.hpp"
#include "pathing/randomPoints.hpp"
#include "pathing/graph.hpp"
#include "graphics/glWindow.hpp"


#include<fstream>
#include<sstream>
#include<string>
namespace graphics {
    class ShaderCompilationError: public std::exception {
    public:
        ShaderCompilationError(GLuint shaderId) {
            m_what = "Shader compilation failed: ";

            GLsizei maxLength;
            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

            char* infoLog = new char[maxLength];
            GLsizei length;
            glGetShaderInfoLog(shaderId, maxLength, &length, infoLog);
            m_what += std::string(infoLog, length);
            delete[] infoLog;
        }
        const char * what() const throw () {
            return m_what.c_str();
        }
    private:
        std::string m_what;
    };

    class ShaderLinkingError: public std::exception {
    public:
        ShaderLinkingError(GLuint shaderId) {
            m_what = "Shader linking failed: ";

            GLsizei maxLength;
            glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

            char* infoLog = new char[maxLength];
            GLsizei length;
            glGetProgramInfoLog(shaderId, maxLength, &length, infoLog);
            m_what += std::string(infoLog, length);
            delete[] infoLog;
        }
        const char * what() const throw () {
            return m_what.c_str();
        }
    private:
        std::string m_what;
    };

    class Shader {
    public:
        Shader(const std::string& src, GLuint shader_type) {
            shader = glCreateShader(shader_type);
            const char * src_ = src.c_str();
            int size = src.size();
            glShaderSource(shader, 1, &src_, &size);
            glCompileShader(shader);

            int isCompiled = false;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

            if(isCompiled == false) {
                throw ShaderCompilationError(shader);
            }
        }

        GLuint id() {
            return shader;
        }
        ~Shader() {
            glDeleteShader(shader);
        }

    private:
        GLuint shader = 0;
    };

    class ShaderProgram {
    public:
        ShaderProgram(const std::string& vert, const std::string& frag)
            : vertexShader(vert, GL_VERTEX_SHADER)
            , fragmentShader(frag, GL_FRAGMENT_SHADER)
        {
            shaderProgram = glCreateProgram();

            // TODO: use consts here and in VAO
            glBindAttribLocation(shaderProgram, 0, "in_position");
            glBindAttribLocation(shaderProgram, 1, "in_color");

            glAttachShader(shaderProgram, vertexShader.id());
            glAttachShader(shaderProgram, fragmentShader.id());

            glLinkProgram(shaderProgram);

            int isLinked = false;
            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);

            if(isLinked == false) {
                throw ShaderLinkingError(shaderProgram);
            }
        }

        void use() {
            glUseProgram(shaderProgram);
        }

        ~ShaderProgram() {
            glUseProgram(0);
            glDetachShader(shaderProgram, vertexShader.id());
            glDetachShader(shaderProgram, fragmentShader.id());
        }
    private:
        GLuint shaderProgram = 0;
        Shader vertexShader;
        Shader fragmentShader;
    };

    struct Point {
        GLfloat x, y, z;
        static const GLsizei length = 3;
        static const GLsizei mem_size = length * sizeof(GLfloat);

    };

    // struct Color {
    //     GLfloat r, g, b, a;
    //     static const GLsizei length = 4;
    //     static const GLsizei mem_size = length * sizeof(GLfloat);
    // };

    class StaticVAO {
    public:
        StaticVAO(const std::vector<Point>& points, const std::vector<Color>& colors) {
            glGenVertexArrays(1, vao);
            glGenBuffers(2, vbo);
            set_points(points);
            set_colors(colors);
        }

        void draw() {
            glBindVertexArray(vao[0]);

            glEnableVertexAttribArray(position_attribute_index);
            glEnableVertexAttribArray(color_attribute_index);

            glDrawArrays(GL_TRIANGLE_FAN, 0, vertex_count);
        }
    private:
        void set_points(const std::vector<Point>& points) {
            glBindVertexArray(vao[0]);
            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

            glBufferData(
                GL_ARRAY_BUFFER, points.size() * Point::mem_size,
                points.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(position_attribute_index, Point::length, GL_FLOAT, GL_FALSE, 0, 0);

            vertex_count = points.size();

        }
        void set_colors(const std::vector<Color>& colors) {
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

        GLuint vbo[2], vao[1];
        GLsizei vertex_count = 0;
        static const int position_attribute_index = 0;
        static const int color_attribute_index = 1;
    };
}

std::string load_file(std::string file_name) {
    std::ifstream f(file_name);
    std::stringstream stream;
    stream << f.rdbuf();

    return stream.str();
}

void render_square(graphics::Renderer& renderer) {
    graphics::ShaderProgram shader = graphics::ShaderProgram(
        load_file("shaders/basic.vert"), load_file("shaders/basic.frag")
    );

    const std::vector<graphics::Point> points = {
        { -0.5,  0.5,  0.5 }, // Top left
        {  0.5,  0.5,  0.5 }, // Top right
        {  0.5, -0.5,  0.5 }, // Bottom right 
        { -0.5, -0.5,  0.5 }, // Bottom left
    };


    const std::vector<graphics::Color> colors = {
        { 0.0, 1.0, 0.0, 1.0 }, // Top left
        { 1.0, 1.0, 0.0, 1.0  }, // Top right
        { 1.0, 0.0, 0.0, 1.0  }, // Bottom right 
        { 0.0, 0.0, 1.0, 1.0  }, // Bottom left
    };

    graphics::StaticVAO square(points, colors);

    shader.use();
    square.draw();

}

bool handle_key_down(const SDL_Event& event, graphics::Renderer& renderer) {
    const graphics::Color red{1.0, 0.0, 0.0, 1.0};
    const graphics::Color green{0.0, 1.0, 0.0, 1.0};
    const graphics::Color blue{0.0, 0.0, 1.0, 1.0};
    switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            return false;
            break;
        case SDLK_r:
            renderer.clear(red);
            renderer.swap();
            break;
        case SDLK_g:
            renderer.clear(green);
            renderer.swap();
            break;
        case SDLK_b:
            renderer.clear(blue);
            renderer.swap();
            break;
        default:
            break;
    }
    return true;
}

void main_loop(graphics::Renderer renderer) {
    bool running = true;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    running = handle_key_down(event, renderer);
                    break;
                default:
                    break;
            }
        }
        render_square(renderer);
        renderer.swap();
    }
}

int main() {
    // initi video
    graphics::Window window;
    graphics::Renderer renderer(window);
    graphics::Color black{0.0, 0.0, 0.0, 1.0};
    renderer.clear(black);
    renderer.swap();

    main_loop(renderer);

    // main
    auto points = pathing::randomPoints(1000, {0, 0}, {100, 100});
    auto graph = pathing::connectPointsWithinThreshold(points, 5);
    pathing::AStar aStar(graph);
    std::cout << aStar.shortest_path(700, 500) << std::endl;
    for(size_t j = 0; j < points.size(); j++) {
        //std::cout << j << " ";
        auto path = aStar.shortest_path(0, j);
        //std::cout << path.nodes.size() << std::endl;
    }

    size_t sum = 0;
    for(size_t i = 0; i < points.size(); i++) {
        sum += graph.edgesFrom(i).size();
    }
    std::cout << points.size() << " " << sum << std::endl;

    return 0;
}