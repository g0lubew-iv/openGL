//
// Created by one_eyed_john on 09/03/23.
//

#include "glfw_system.hxx"
#include <GLFW/glfw3.h>
#include <stdexcept>

snake::GLFWSystem::GLFWSystem() {
    if (!is_initialized()) {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW!");
        }
        is_initialized_ = true;
    }
}

snake::GLFWSystem::~GLFWSystem() {
    if (is_initialized()) {
        glfwTerminate();
        is_initialized_ = false;
    }
}

bool snake::GLFWSystem::is_initialized() noexcept {
    return is_initialized_;
}

