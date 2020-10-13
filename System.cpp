#include "System.h"
#include "GLFW/glfw3.h"
#include <stdexcept>

System::System() {
    logDiagnostics();
    glfwSetErrorCallback(onError);
    auto result = glfwInit();
    if (result == GLFW_FALSE) {
        throw std::runtime_error("Failed to initialize glfw \n");
    }
}

System::~System() {
    glfwTerminate();
}

std::string_view System::getVersionString() const noexcept {
    return glfwGetVersionString();
}

void System::onError(int code, const char* description) noexcept {
    printf("GLFW Error (%i) %s\n", code, description);
}

void System::logDiagnostics() const noexcept {
    printf("Compiled against GLFW %i.%i.%i\n", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    printf("Running against GLFW %i.%i.%i\n", major, minor, revision);
}