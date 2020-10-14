#pragma once
#include <string_view>
#include <filesystem>
//TODO: can't be passed by value, since running the destructor == deleting a global resource. This design is too fragile.
class Shader
{
    using Path = std::filesystem::path;
    using GLuint = unsigned int;
public:    
    GLuint ID;
    Shader(std::string_view vertexShaderSource, std::string_view fragmentShaderSource);
    Shader(Path vertexShader, Path fragmentShader);
    ~Shader();
    void use() const noexcept;
    
    void setBool(const std::string_view name, bool value) const noexcept;
    void setInt(const std::string_view name, int value) const noexcept;
    void setFloat(std::string_view name, float value) const noexcept;
    void setFloat(std::string_view name, double value) const noexcept;

private:
    static bool checkStatus(GLuint obj) noexcept;
    GLuint buildShader(std::string_view vertexShaderSource, std::string_view fragmentShaderSource) const;
};
