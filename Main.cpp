#include "System.h"
#include "Window.h"
#include "Shader.h"
#include <filesystem>
#include <iostream>
#include <vector>
#include <cmath>
using path = std::filesystem::path;
/*
* Picking up a bit of OpenGL in the evenings, following the tutorials over at https://learnopengl.com/
*/

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;
const std::vector<GLfloat> vertices = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
};

template <class T>
inline void myBufferData(GLenum target, const std::vector<T>& v, GLenum usage) {
    glBufferData(target, v.size() * sizeof(T), v.data(), usage);
}

void processInput(GLFWwindow* window) noexcept  {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void render(const Shader& shader, GLuint VAO) noexcept {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shader.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //glDrawElements(GL_TRIANGLES, static_cast<GLuint>(elementCount), GL_UNSIGNED_INT, 0);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]){    
    System system;    
    Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL");
    window.makeCurrent();   
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD \n");
    }  
    Shader shader(path("shaders/shader02.vs"), path("shaders/shader02.fs"));        
    shader.use();

    GLuint VBO, VAO; //Vertex Buffer Object (verts), Vertex Array Object (vertex attribute calls)
    glGenVertexArrays(1, &VAO); //Vertex Array Object (vertex attribute calls)
    glGenBuffers(1, &VBO); //Vertex Buffer Object (verts)
    glBindVertexArray(VAO); //bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
       
    //copy our verts into an opengl buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    myBufferData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW);
        
    //then set the vertex attributes pointers
    constexpr GLuint vertLocation = 0;
    constexpr GLuint colorLocation = 1;
    constexpr GLint size = 3; //the size of the vertex and color attributes, 3 floats each
    constexpr GLenum type = GL_FLOAT;
    constexpr GLboolean normalized = GL_FALSE;
    constexpr GLsizei stride = 6 * sizeof(float);
    const void* vertsOffset = 0;
    const void* colorOffset = (void*)(size * sizeof(float));
    glVertexAttribPointer(vertLocation, size, type, normalized, stride, vertsOffset);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(colorLocation, size, type, normalized, stride, colorOffset);
    glEnableVertexAttribArray(1);
    
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); //unbind VAO 
       
    float range = 1;
    float count = 0;
    while (!window.shouldClose()) {
        float offset = std::sinf(count += 0.000125) * range;
        processInput(window.getPtr());        
        shader.setFloat("xOffset", offset);
        render(shader, VAO);
        window.swapBuffer();
        glfwPollEvents();
    }    

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);    
    return 0;
}
