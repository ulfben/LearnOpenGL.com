#include "glad/glad.h"  // include glad to get all the required OpenGL headers
#include "GLFW/glfw3.h"

#include "System.h"
#include "Window.h"
#include "Shader.h"
#include "Texture.h"
#include <filesystem>
#include <iostream>
#include <vector>
#include <cmath>

/*
* Picking up a bit of OpenGL in the evenings, following the tutorials over at https://learnopengl.com/
*/
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;
#include "models/textured_square.verts"

enum Stride : GLsizei {
    XYZ = 3 * sizeof(float),
    XYZRGB = 6 * sizeof(float),
    XYZRGBST = 8 * sizeof(float)
};

namespace Components {
    constexpr GLint RGB = 3;
    constexpr GLint XYZ = 3;
    constexpr GLint ST = 2;
};

template<typename T>
constexpr T PI = T(3.141592653589793238462643383279502884L);

template<typename T>
constexpr T TO_DEGREES = 180.0L / PI<T>;

template<typename T>
constexpr T TO_RADIANS = PI<T> / 180.0L;

template<typename T>
constexpr T radians(const T degrees) noexcept { return TO_RADIANS<T> * degrees; };

template <class T>
inline void myBufferData(GLenum target, const std::vector<T>& v, GLenum usage) noexcept {
    glBufferData(target, v.size() * sizeof(T), v.data(), usage);
}

void processInput(GLFWwindow* window) noexcept {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void renderStart(const Shader& shader, GLuint VAO) noexcept {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAO);
    shader.use();
}

void render(const Shader& shader, GLuint VAO) noexcept {
    renderStart(shader, VAO);        
    glDrawArrays(GL_TRIANGLES, 0, 3);        
}

void render(const Shader& shader, GLuint VAO, const std::vector<GLuint>& elements) noexcept {
    renderStart(shader, VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(elements.size()), GL_UNSIGNED_INT, 0);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]){    
    using Path = std::filesystem::path;

    System system;    
    Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL");
    window.makeCurrent();   
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD \n");
    }  
    Shader shader(Path("shaders/shader03.vs"), Path("shaders/shader03.fs"));        
    shader.use();     

    GLuint VBO, VAO, EBO; //Vertex Buffer Object (verts), Vertex Array Object (vertex attribute calls), Element Buffer Object (indexes)
    glGenVertexArrays(1, &VAO); //Vertex Array Object (vertex attribute calls)
    glGenBuffers(1, &VBO); //Vertex Buffer Object (verts)
    glGenBuffers(1, &EBO); //Element Buffer Object (indices)
    glBindVertexArray(VAO); //bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
       
    //copy our verts into an opengl buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    myBufferData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    myBufferData(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW);
        
    //then set the vertex attributes pointers
    constexpr GLuint vertLocation = 0;
    constexpr GLuint colorLocation = 1;
    constexpr GLuint textureLocation = 2;
    constexpr GLint size = 3; //the size of the vertex and color attributes, 3 floats each
    constexpr GLenum type = GL_FLOAT;
    constexpr GLboolean normalized = GL_FALSE;
    constexpr GLsizei stride = Stride::XYZRGBST;
    
    const void* vertsOffset = 0; 
    glVertexAttribPointer(vertLocation, Components::XYZ, type, normalized, stride, vertsOffset);
    glEnableVertexAttribArray(vertLocation);

    const void* colorOffset = (void*)(Stride::XYZ); //point us past the verts
    glVertexAttribPointer(colorLocation, Components::RGB, type, normalized, stride, colorOffset);
    glEnableVertexAttribArray(colorLocation);

    const void* textureOffset = (void*)(Stride::XYZRGB); //point us past the verts and colors
    glVertexAttribPointer(textureLocation, Components::ST, type, normalized, stride, textureOffset);
    glEnableVertexAttribArray(textureLocation);
    
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); //unbind VAO       
    
    GLuint texture;
    {
        Texture t{ Path("textures/container.jpg") };        
        const GLuint mipmapLevel = 0;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, mipmapLevel, GL_RGB, t.width(), t.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, t.pixels());
        glGenerateMipmap(GL_TEXTURE_2D);        
    }
    const auto RANGE = 1.0; //amplitude of our sine wave (how far to travel)
    const auto SPEED = 360.0 / 2.0; //I want to cover a full revolution (360 degrees) in 2 seconds.
    while (!window.shouldClose()) {        
        processInput(window.getPtr());
        //const auto angle = std::fmod(glfwGetTime() * SPEED, 360); //turn linear, ever growing, timestamp into 0-359 range                
        //const auto offset = std::sin(radians(angle)) * RANGE;        
        //shader.setFloat("xOffset", offset);
        render(shader, VAO, indices);
        window.swapBuffer();
        glfwPollEvents();
    }    

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    return 0;
}
