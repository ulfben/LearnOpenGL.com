#pragma once
#include <string_view>
#include <filesystem>

class Shader
{
    using Path = std::filesystem::path;
    using GLuint = unsigned int;
public:    
    GLuint ID;    
    Shader(std::string_view vertexShaderSource, std::string_view fragmentShaderSource);
    Shader(Path vertexShader, Path fragmentShader);
    
    void use() const noexcept;
    
    void setBool(const std::string_view name, bool value) const noexcept;
    void setInt(const std::string_view name, int value) const noexcept;
    void setFloat(std::string_view name, float value) const noexcept;
    void setFloat(std::string_view name, double value) const noexcept;

private:
    static bool checkStatus(GLuint obj) noexcept;
    GLuint buildShader(std::string_view vertexShaderSource, std::string_view fragmentShaderSource) const;
};
