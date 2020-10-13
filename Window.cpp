#pragma once
#include "GLFW/glfw3.h"
#include "Window.h"
#include <stdexcept>
#include <string>

Window::Window(const unsigned int width, const unsigned int height, const std::string title)
    : _width(width)
    , _height(height)
    , _title(title)
{
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

Window::~Window()
{
    glfwDestroyWindow(_window);
}

void Window::swapBuffer() const noexcept
{
    glfwSwapBuffers(_window);
}

bool Window::shouldClose() const noexcept
{
    return glfwWindowShouldClose(_window);
}

GLFWwindow* Window::getPtr() const noexcept
{
    return _window;
}

void Window::makeCurrent() const noexcept
{
    glfwMakeContextCurrent(_window);
}

void Window::onFrameBufferSize(int width, int height) const noexcept
{
    glViewport(0, 0, width, height);
}

void Window::setCallbacks() const noexcept
{
    auto frameBufferSizeCallback = [](GLFWwindow* w, int width, int height) {
        auto ptr = static_cast<Window*>(glfwGetWindowUserPointer(w));
        if (ptr) {
            ptr->onFrameBufferSize(width, height);
        }
    };
    glfwSetFramebufferSizeCallback(_window, frameBufferSizeCallback);
}
