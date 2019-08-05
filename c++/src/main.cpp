
#include <iostream>
#include <stddef.h>
#include <vector>
#include <exception>

#include "pathing/a_star.hpp"
#include "pathing/randomPoints.hpp"
#include "pathing/graph.hpp"

#include <SDL2/SDL.h>
#include <GL/glew.h>

namespace graphics {
    class SDLException: public std::exception {
    public:
        SDLException() {
            m_what = SDL_GetError();
            SDL_ClearError();
        }
        const char * what() const throw () {
            return m_what;
        }
    private:
        const char * m_what;
    };

    struct Color {
        Color(float r, float g, float b, float a = 1.0): r(r), g(g), b(b), a(a) {
        }
        float r;
        float g;
        float b;
        float a;
    };

    class Window {
    public:    
        Window() {
            if (SDL_Init(SDL_INIT_VIDEO) < 0)
            {
                throw SDLException();
            }

            window = SDL_CreateWindow(
                "A Star",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                512,
                512,
                SDL_WINDOW_OPENGL
            );

            if (window == nullptr) {
                throw SDLException();
            }

            // ES for mobile, CORE for new
            try_setting_attribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            try_setting_attribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            try_setting_attribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
            try_setting_attribute(SDL_GL_DOUBLEBUFFER, 1);

            SDL_GLContext context = SDL_GL_CreateContext(window);
            if (context == nullptr) {
                throw SDLException();
            };

            glewExperimental = GL_TRUE;
            glewInit();

            SDL_GL_SetSwapInterval(1);
        }
        ~Window() {
            SDL_GL_DeleteContext(context);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }

        Window(const Window&) = delete; // no copies
        Window(Window&&) = delete; // no move
        Window& operator=(const Window&) = delete; // no assignments
        Window& operator=(Window&&) = delete; // no move assignment

        void swap() {
            SDL_GL_SwapWindow(window);
        }
    private:
        SDL_Window* window = nullptr;
        SDL_GLContext* context = nullptr;

        void try_setting_attribute(SDL_GLattr attr, int value) {
            if (SDL_GL_SetAttribute(attr, value) != 0) {
                throw "Failed to init OpenGL\n";
            }
        }
    };

    class Renderer {
    public:
        Renderer(Window& window): m_window(window) {
        }
        void clear(Color color) {
            glClearColor(color.r, color.g, color.b, color.a);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        void swap() {
            m_window.swap();
        }
    private:
        Window& m_window;
    };
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