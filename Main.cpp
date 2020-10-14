#include "glad/glad.h"  // include glad to get all the required OpenGL headers
#include "GLFW/glfw3.h"
#include "System.h"
#include "Window.h"
#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include <filesystem>
#include <iostream>
#include <vector>
#include "Math.h"
/*
* Picking up a bit of OpenGL in the evenings, following the tutorials over at https://learnopengl.com/
*/
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;
#include "models/textured_square.verts"

void startFrame(const Window& window) noexcept {
    window.processInput();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);   
}

void render(const Model& model) noexcept {        
    model.render();     
}

void endFrame(const Window& window) noexcept {
    window.swapBuffer();
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]){    
    using Path = std::filesystem::path;  
    System system;    
    Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL");
    window.makeCurrent();   
    window.vsync(true);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD \n");
    }  
    
    Shader shader(Path("shaders/shader03.vs"), Path("shaders/shader03.fs"));   
    Texture texture{ Image(Path("textures/container.jpg")) };
    Model box(vertices, indices, &shader, &texture); //TODO: probably split "Model" into several components. Material (shader, texture), Mesh (verts, indices), Entity (position, scale etc)? 
        
    const auto RANGE = 1.0; //amplitude of our sine wave (how far to travel)
    const auto SPEED = 360.0 / 2.0; //I want to cover a full revolution (360 degrees) in 2 seconds.
    while (!window.shouldClose()) {               
        const auto angle = std::fmod(glfwGetTime() * SPEED, 360); //turn linear, ever growing, timestamp into 0-359 range                
        const auto offset = std::sin(Math::radians(angle)) * RANGE;        
        shader.setFloat("xOffset", offset);       
        startFrame(window);
        render(box); 
        endFrame(window);
        glfwPollEvents();
    }       
    return 0;
}
