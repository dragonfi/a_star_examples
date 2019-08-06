#pragma once

#include <exception>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "types.hpp"

namespace graphics {
    class SDLException: public std::exception {
    public:
        SDLException();
        const char * what() const throw ();
    private:
        const char * m_what;
    };

    class Window {
    public:
        Window(uint width = 512, uint height = 512);

        Window(const Window&) = delete; // no copies
        Window(Window&&) = delete; // no move
        Window& operator=(const Window&) = delete; // no assignments
        Window& operator=(Window&&) = delete; // no move assignment

        ~Window();

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