
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

    class VAO {

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
    shader.use();

    GLuint positionAttributeIndex = 0;
    GLuint colorAttributeIndex = 1;

    const int points = 4;
    const int floatsPerPoint = 3;
    const int floatsPerColor = 4;
    const GLfloat diamond[points][floatsPerPoint] = {
        { -0.5,  0.5,  0.5 }, // Top left
        {  0.5,  0.5,  0.5 }, // Top right
        {  0.5, -0.5,  0.5 }, // Bottom right 
        { -0.5, -0.5,  0.5 }, // Bottom left
    };


    const GLfloat colors[points][floatsPerColor] = {
        { 0.0, 1.0, 0.0, 1.0 }, // Top left
        { 1.0, 1.0, 0.0, 1.0  }, // Top right
        { 1.0, 0.0, 0.0, 1.0  }, // Bottom right 
        { 0.0, 0.0, 1.0, 1.0  }, // Bottom left
    };

    GLuint vbo[2], vao[1];
    glGenBuffers(2, vbo);
    glGenVertexArrays(1, vao);

    glBindVertexArray(vao[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, ( points * floatsPerPoint) * sizeof(GLfloat), diamond, GL_STATIC_DRAW);
    glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(positionAttributeIndex);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, ( points * floatsPerColor) * sizeof(GLfloat), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(colorAttributeIndex, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorAttributeIndex);

    shader.use();

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Render

    glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Invoke glDrawArrays telling that our data is a line loop and we want to draw 2-4 vertexes
	glDrawArrays(GL_LINE_LOOP, 0, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

bool handle_key_down(const SDL_Event& event, graphics::Renderer& renderer) {
    const graphics::Color red(1.0, 0.0, 0.0);
    const graphics::Color green(0.0, 1.0, 0.0);
    const graphics::Color blue(0.0, 0.0, 1.0);
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
    graphics::Color black(0.0, 0.0, 0.0);
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