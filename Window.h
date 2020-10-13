#pragma once
#include <string>
struct GLFWwindow;
class Window {       
public:
    Window(unsigned int width, unsigned int height, std::string title);
    ~Window();

    void vsync(bool on) const noexcept;
    void swapBuffer() const noexcept;
    bool shouldClose() const noexcept;
    GLFWwindow* getPtr() const noexcept;
    void makeCurrent() const noexcept;

private:
    std::string _title;
    unsigned int _width = 0;
    unsigned int _height = 0;    
    GLFWwindow* _window = nullptr;

    void onFrameBufferSize(int width, int height) const noexcept;
    void setCallbacks() const noexcept;
};
