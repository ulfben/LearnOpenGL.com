#pragma once
#include "glad/glad.h"
#include "FileReader.h"
#include <string>
#include <iostream>
#include <string_view>
#include <filesystem>

class Shader
{
    using path = std::filesystem::path;
public:    
    GLuint ID;    
    Shader(std::string_view vertexShaderSource, std::string_view fragmentShaderSource) {
        ID = buildShader(vertexShaderSource, fragmentShaderSource);
    }
    Shader(path vertexShader, path fragmentShader) {
        FileReader vShader(vertexShader);
        FileReader fShader(fragmentShader);       
        ID = buildShader(vShader.getContent(), fShader.getContent());        
    }      
    
    void use() const noexcept {
        glUseProgram(ID);
    }
    
    void setBool(const std::string& name, bool value) const noexcept {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (GLint)value);
    }
    void setInt(const std::string& name, int value) const noexcept {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(std::string_view name, float value) const noexcept {
        glUniform1f(glGetUniformLocation(ID, name.data()), value);
    }

private:
    static bool checkStatus(GLuint obj) noexcept {
        GLint status = GL_FALSE;
        if (glIsShader(obj)) glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
        if (glIsProgram(obj)) glGetProgramiv(obj, GL_LINK_STATUS, &status);
        if (status == GL_TRUE) return true;
        GLchar log[1 << 15] = { 0 };
        if (glIsShader(obj)) glGetShaderInfoLog(obj, sizeof(log), NULL, log);
        if (glIsProgram(obj)) glGetProgramInfoLog(obj, sizeof(log), NULL, log);
        std::cerr << log << std::endl;
        return false;
    }

    GLuint buildShader(std::string_view vertexShaderSource, std::string_view fragmentShaderSource) const {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* c_str = vertexShaderSource.data();
        glShaderSource(vertexShader, 1, &c_str, NULL);
        glCompileShader(vertexShader);
        if (!checkStatus(vertexShader)) {
            throw std::runtime_error("Failed to compile vertexShader\n");
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        c_str = fragmentShaderSource.data();
        glShaderSource(fragmentShader, 1, &c_str, NULL);
        glCompileShader(fragmentShader);
        if (!checkStatus(fragmentShader)) {
            throw std::runtime_error("Failed to compile fragmentShader\n");
        }

        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        if (!checkStatus(shaderProgram)) {
            throw std::runtime_error("Failed to link the shader\n");
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return shaderProgram;
    };
};
