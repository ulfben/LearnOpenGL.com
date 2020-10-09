#pragma once
#include "glad/glad.h" // include glad to get all the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:    
    GLuint ID;    
    Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
        ID = buildShader(vertexShaderSource, fragmentShaderSource);
    }
    Shader(const char* vertexPath, const char* fragmentPath) {
        // 1. retrieve the vertex/fragment source code from filePath        
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);        
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        const auto vertexCode = vShaderStream.str();
        const auto fragmentCode = fShaderStream.str();        
       /* catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }*/
       ID = buildShader(vertexCode, fragmentCode);        
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
    void setFloat(const std::string& name, float value) const noexcept {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
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

    GLuint buildShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) const {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* c_str = vertexShaderSource.c_str();
        glShaderSource(vertexShader, 1, &c_str, NULL);
        glCompileShader(vertexShader);
        if (!checkStatus(vertexShader)) {
            throw std::runtime_error("Failed to compile vertexShader\n");
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* c_str2 = fragmentShaderSource.c_str();
        glShaderSource(fragmentShader, 1, &c_str2, NULL);
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
