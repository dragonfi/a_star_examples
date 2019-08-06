#pragma once

#include<exception>
#include<string>
#include<GL/glew.h>

namespace graphics {
    class ShaderCompilationError: public std::exception {
    public:
        ShaderCompilationError(GLuint shaderId);
        const char * what() const throw ();
    private:
        std::string m_what;
    };

    class ShaderLinkingError: public std::exception {
    public:
        ShaderLinkingError(GLuint shaderId);
        const char * what() const throw ();
    private:
        std::string m_what;
    };

    class Shader {
    public:
        Shader(const std::string& src, GLuint shader_type);
        GLuint id();
        ~Shader();
    private:
        GLuint shader = 0;
    };

    class ShaderProgram {
    public:
        ShaderProgram(const std::string& vert, const std::string& frag);
        void use();
        ~ShaderProgram();
    private:
        GLuint shaderProgram = 0;
        Shader vertexShader;
        Shader fragmentShader;
    };
}