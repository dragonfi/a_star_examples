
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
        }
        const char * what() const throw () {
            return m_what;
        }
    private:
        const char * m_what;
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

        void clear() {
            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        void swap() {
            SDL_GL_SwapWindow(window);
        }
    private:
        SDL_Window* window;
        SDL_GLContext* context;

        void try_setting_attribute(SDL_GLattr attr, int value) {
            if (SDL_GL_SetAttribute(attr, value) != 0) {
                throw "Failed to init OpenGL\n";
            }
        }
    };
}

int main() {
    // initi video
    graphics::Window window;
    window.clear();
    window.swap();

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