#include "glWindow.hpp"


namespace graphics {
    SDLException::SDLException() {
        m_what = SDL_GetError();
        SDL_ClearError();
    }

    const char * SDLException::what() const throw () {
        return m_what;
    }

    Color::Color(float r, float g, float b, float a): r(r), g(g), b(b), a(a) {
    }

    Window::Window() {
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

    Window::~Window() {
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void Window::swap() {
        SDL_GL_SwapWindow(window);
    }

    void Window::try_setting_attribute(SDL_GLattr attr, int value) {
        if (SDL_GL_SetAttribute(attr, value) != 0) {
            throw "Failed to init OpenGL\n";
        }
    }

    Renderer::Renderer(Window& window): m_window(window) {
    }
    void Renderer::clear(Color color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::swap() {
        m_window.swap();
    }
}