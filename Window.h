#pragma once
#include "System.h"
#include <string>
class Window {
public:
    Window(const unsigned int width, const unsigned int height, const std::string title) : _width(width), _height(height), _title(title){
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        _window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);
        if (!_window) {
            throw std::runtime_error("Failed to create GLFW window \n");
        }
        glfwSetWindowUserPointer(_window, this); //save this instance, so we can use the pointer for our callbacks. 
        setCallbacks();        
    }

    ~Window() {
        glfwDestroyWindow(_window);
    }
    
    void swapBuffer() const noexcept {
        glfwSwapBuffers(_window);
    }

    bool shouldClose() const noexcept {
        return glfwWindowShouldClose(_window);
    }

    GLFWwindow* getPtr() const noexcept {
        return _window;
    }

    void makeCurrent() const noexcept {
        glfwMakeContextCurrent(_window);
    }

private:    
    unsigned int _width = 0;
    unsigned int _height = 0;
    std::string _title;
    GLFWwindow* _window = nullptr;

    void onFrameBufferSize(int width, int height){
        glViewport(0, 0, width, height);
    }

    void setCallbacks() {
        auto frameBufferSizeCallback = [](GLFWwindow* w, int width, int height) {
            auto ptr = static_cast<Window*>(glfwGetWindowUserPointer(w));
            if (ptr) {
                ptr->onFrameBufferSize(width, height);
            }
        };
        glfwSetFramebufferSizeCallback(_window, frameBufferSizeCallback);
    }

};