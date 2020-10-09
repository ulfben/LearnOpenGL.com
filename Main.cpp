#include "System.h"
#include "Window.h"
#include "Shader.h"
#include <filesystem>
#include <iostream>
#include <vector>
using path = std::filesystem::path;
/*
* Picking up a bit of OpenGL in the evenings, following the tutorials over at https://learnopengl.com/
*/

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;
const std::vector<GLfloat> vertices = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
};
const std::vector<GLuint> indices = {
    0, 1, 3,  // first tri
    1, 2, 3   // second tri
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

void render(const Shader& shader, GLuint VAO, size_t elementCount) noexcept {    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shader.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLuint>(elementCount), GL_UNSIGNED_INT, 0);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]){    
    System system;    
    Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL");
    window.makeCurrent();   
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD \n");
    }  
    Shader shader(path("shaders/shader01.vs"), path("shaders/shader01.fs"));        
    shader.use();

    GLuint VBO, VAO, EBO; //Vertex Buffer Object (verts), Vertex Array Object (vertex attribute calls), Element Buffer Object (indices)
    glGenVertexArrays(1, &VAO); //Vertex Array Object (vertex attribute calls)
    glGenBuffers(1, &VBO); //Vertex Buffer Object (verts)
    glGenBuffers(1, &EBO); //Element Buffer Object(indices)
    glBindVertexArray(VAO); //bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
       
    //copy our verts into an opengl buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    myBufferData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);    
    myBufferData(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW);
    
    //then set the vertex attributes pointers
    constexpr GLuint index = 0;
    constexpr GLint size = 3; //the size of the vertex attribute, 3 floats
    constexpr GLenum type = GL_FLOAT;
    constexpr GLboolean normalized = GL_FALSE;
    constexpr GLsizei stride = size * sizeof(float);
    const void* pointer = 0;
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    glEnableVertexAttribArray(0);
    
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); //unbind VAO 
    //do NOT unbind the EBO while a VAO is active     
      
    while (!window.shouldClose()) {
        processInput(window.getPtr());
        render(shader, VAO, indices.size());
        window.swapBuffer();
        glfwPollEvents();
    }    

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);    
    return 0;
}
