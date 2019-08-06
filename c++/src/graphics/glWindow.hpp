#pragma once

#include <exception>
#include <SDL2/SDL.h>
#include <GL/glew.h>

namespace graphics {
    class SDLException: public std::exception {
    public:
        SDLException();
        const char * what() const throw ();
    private:
        const char * m_what;
    };

    struct Color {
        GLfloat r, g, b, a;
        static const GLsizei length = 4;
        static const GLsizei mem_size = length * sizeof(GLfloat);
    };

    class Window {
    public:
        Window();
        ~Window();

        Window(const Window&) = delete; // no copies
        Window(Window&&) = delete; // no move
        Window& operator=(const Window&) = delete; // no assignments
        Window& operator=(Window&&) = delete; // no move assignment

        void swap();
    private:
        SDL_Window* window = nullptr;
        SDL_GLContext* context = nullptr;

        void try_setting_attribute(SDL_GLattr attr, int value);
    };

    class Renderer {
    public:
        Renderer(Window& window);

        void clear(Color color);
        void swap();
    private:
        Window& m_window;
    };
}