//
// Created by one_eyed_john on 09/03/23.
//

#include <window.hxx>
#include <glfw_system.hxx>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>

snake::Window::Window(const DisplayData &display_data) {
    if (!GLFWSystem::is_initialized()) {
        throw std::runtime_error("GLFW must be initialized!");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    window = glfwCreateWindow(display_data.size.x, display_data.size.y,
                              display_data.title.c_str(), nullptr, nullptr);

    if (window == nullptr) {
        throw std::runtime_error("Failed to create window!");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        throw std::runtime_error("Failed to load OpenGL!");
    }
}

snake::Window::~Window() {
    if (window != nullptr) {
        glfwDestroyWindow(window);
    }
}

snake::Window::Window(snake::Window &&other) noexcept {
    window = other.window;
    other.window = nullptr;
}

snake::Window &snake::Window::operator=(snake::Window &&other) noexcept {
    window = other.window;
    other.window = nullptr;
    return *this;
}
