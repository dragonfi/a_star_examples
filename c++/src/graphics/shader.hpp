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

        Shader(const Shader&) = delete;
        Shader(Shader&&) = delete;
        Shader& operator=(const Shader&) = delete;
        Shader& operator=(Shader&&) = delete;

        ~Shader();

        GLuint id();
    private:
        GLuint shader = 0;
    };

    class ShaderProgram {
    public:
        ShaderProgram(const std::string& vert, const std::string& frag);

        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram(ShaderProgram&&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        ShaderProgram& operator=(ShaderProgram&&) = delete;

        ~ShaderProgram();

        void use();
    private:
        GLuint shaderProgram = 0;
        Shader vertexShader;
        Shader fragmentShader;
    };
}