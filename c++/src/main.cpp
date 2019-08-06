
#include <iostream>
#include <stddef.h>
#include <vector>
#include <exception>

#include <SDL2/SDL.h>

#include "pathing/a_star.hpp"
#include "pathing/randomPoints.hpp"
#include "pathing/graph.hpp"
#include "graphics/glWindow.hpp"

#include "ostream_helpers/ostream_helpers.hpp"


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
        StaticVAO(GLuint shape, const std::vector<Point>& points, const std::vector<Color>& colors)
            : m_shape(shape)
        {
            glGenVertexArrays(1, vao);
            glGenBuffers(2, vbo);
            set_points(points);
            set_colors(colors);
        }

        void draw() {
            draw(m_shape);
        }

        void draw(GLuint shape) {
            glBindVertexArray(vao[0]);

            glEnableVertexAttribArray(position_attribute_index);
            glEnableVertexAttribArray(color_attribute_index);

            glDrawArrays(shape, 0, vertex_count);
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
        GLuint m_shape;
        GLsizei vertex_count = 0;
        static const int position_attribute_index = 0;
        static const int color_attribute_index = 1;
    };

    class Scene {
    public:
        Renderer& renderer;
        ShaderProgram& shader;
        std::vector<StaticVAO>& vaos;
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

    graphics::StaticVAO square(GL_TRIANGLE_FAN, points, colors);

    shader.use();
    // GL_LINES, GL_TRIANGLE_FAN, GL_LINES, GL_LINE_LOOP
    square.draw(GL_LINE_LOOP);

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

void main_loop(graphics::Scene scene) {
    bool running = true;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    running = handle_key_down(event, scene.renderer);
                    break;
                default:
                    break;
            }
        }
        //render_square(renderer);
        // renderer.swap();
        scene.renderer.clear(graphics::Color{0.0, 0.0, 0.0, 1.0});
        scene.shader.use();
        for(graphics::StaticVAO vao : scene.vaos) {
            vao.draw();
        }
        scene.renderer.swap();
    }
}

int main() {
    // initi video
    graphics::Window window;
    graphics::Renderer renderer(window);
    graphics::Color black{0.0, 0.0, 0.0, 1.0};
    renderer.clear(black);
    renderer.swap();

    auto points = pathing::randomPoints(1000, {-1.0, -1.0}, {2.0, 2.0});
    auto graph = pathing::connectPointsWithinThreshold(points, 0.08);
    pathing::AStar aStar(graph);
    pathing::Path visualized_path = aStar.shortest_path(500, 700);
    std::cout << visualized_path << std::endl;

    graphics::ShaderProgram shader = graphics::ShaderProgram(
        load_file("shaders/basic.vert"), load_file("shaders/basic.frag")
    );

    std::vector<graphics::StaticVAO> vaos;

    {
        std::vector<graphics::Point> positions;
        std::vector<graphics::Color> colors;

        for(auto node: graph.nodes()) {
            positions.push_back({static_cast<GLfloat>(node.x), static_cast<GLfloat>(node.y), 0.0});
            colors.push_back({1.0, 1.0, 1.0, 1.0});
        }

        vaos.push_back(
            graphics::StaticVAO(GL_POINTS, positions, colors)
        );
    }

    {
        std::vector<graphics::Point> positions;
        std::vector<graphics::Color> colors;
        for(pathing::Edge edge: graph.edges()) {
            pathing::Vec2 source = graph.nodes()[edge.source];
            pathing::Vec2 dest = graph.nodes()[edge.dest];
            positions.push_back({static_cast<GLfloat>(source.x), static_cast<GLfloat>(source.y), 0.0});
            positions.push_back({static_cast<GLfloat>(dest.x), static_cast<GLfloat>(dest.y), 0.0});
            colors.push_back({0.5, 0.5, 0.5, 1.0});
            colors.push_back({0.5, 0.5, 0.5, 1.0});
        }

        vaos.push_back(
            graphics::StaticVAO(GL_LINES, positions, colors)
        );
    }

    {
        std::vector<graphics::Point> positions;
        std::vector<graphics::Color> colors;
        for(pathing::Index index: visualized_path.nodes) {
            pathing::Vec2 node = graph.nodes()[index];
            positions.push_back({static_cast<GLfloat>(node.x), static_cast<GLfloat>(node.y), 0.0});
            colors.push_back({1.0, 1.0, 0.0, 1.0});
        }

        vaos.push_back(
            graphics::StaticVAO(GL_LINE_STRIP, positions, colors)
        );
    }

    graphics::Scene scene{renderer, shader, vaos};

    main_loop(scene);

    // main



    /* for(size_t j = 0; j < points.size(); j++) {
        //std::cout << j << " ";
        auto path = aStar.shortest_path(0, j);
        //std::cout << path.nodes.size() << std::endl;
    }

    size_t sum = 0;
    for(size_t i = 0; i < points.size(); i++) {
        sum += graph.edgesFrom(i).size();
    }
    std::cout << points.size() << " " << sum << std::endl;
*/
    return 0;
}