#include "System.h"
#include "Window.h"
#include <iostream>
#include <vector>
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

const char* vertexShaderSource = R"(#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
})";

const char* fragmentShaderSource =
R"(#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
})";

template <class T>
inline void myBufferData(GLenum target, const std::vector<T>& v, GLenum usage) {
    glBufferData(target, v.size() * sizeof(T), v.data(), usage);
}

void processInput(GLFWwindow* window) noexcept  {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

static bool checkStatus(GLuint obj) noexcept{
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

GLuint buildShader() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    if (!checkStatus(vertexShader)) {
        throw std::runtime_error("Failed to compile vertexShader\n");
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
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


void render(GLuint shader, GLuint VAO, size_t elementCount) noexcept {    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLuint>(elementCount), GL_UNSIGNED_INT, 0);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]){
    System system;    
    Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL");
    window.makeCurrent();    

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD \n";
        return -1;
    }

    GLuint shaderProgram = buildShader();
        
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
        render(shaderProgram, VAO, indices.size());
        window.swapBuffer();
        glfwPollEvents();
    }    

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    return 0;
}
