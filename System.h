#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <stdexcept>
#include <string_view>

class System {
public: 
	System() {
        logDiagnostics();
        glfwSetErrorCallback(onError);
        auto result = glfwInit();
        if (result == GLFW_FALSE) {            
            throw std::runtime_error("Failed to initialize glfw \n");
        }                
	}


	~System() {
        glfwTerminate();
	}

    std::string_view getVersionString() const noexcept {
        return glfwGetVersionString();
    }

    static void onError(int code, const char* description){
        printf("GLFW Error (%i) %s\n", code, description);
    }
private:
    void logDiagnostics() const noexcept{
        printf("Compiled against GLFW %i.%i.%i\n", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);
        int major, minor, revision;
        glfwGetVersion(&major, &minor, &revision);
        printf("Running against GLFW %i.%i.%i\n", major, minor, revision);
    }
};