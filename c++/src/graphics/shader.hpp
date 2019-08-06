#pragma once

#include<exception>
#include<string>
#include<GL/glew.h>

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
}