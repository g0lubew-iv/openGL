//
// Created by one_eyed_john on 09/03/23.
//

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

const char *vertex_shader_source =
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "uniform mat4 mvp;\n"
        "void main() {\n"
        "   gl_Position = mvp * vec4(position, 1);\n"
        "}\0";

const char *fragment_shader_source =
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main() {\n"
        "   color = vec4(1, 0.8, 0.1, 1);\n"
        "}\n\0";

struct Vertex {
    float x;
    float y;
    float z;
};

Vertex vertices[] = {
        {1.f,  1.f,  1.f},
        {1.f,  -1.f, 1.f},
        {-1.f, -1.f, 1.f},
        {-1.f, 1.f,  1.f},
        {1.f,  1.f,  -1.f},
        {1.f,  -1.f, -1.f},
        {-1.f, -1.f, -1.f},
        {-1.f, 1.f,  -1.f},
};

unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3,
        4, 5, 1,
        4, 1, 0,
        7, 6, 5,
        7, 5, 4,
        7, 6, 2,
        7, 2, 3,
        7, 0, 3,
        7, 4, 0,
        6, 5, 1,
        6, 1, 2,
};

glm::ivec2 window_size = {900, 600};

GLFWwindow *initialize_window();

unsigned int make_program();

int main() {
    auto window = initialize_window();

    unsigned int program = make_program();

    unsigned int vertex_buffer;
    glCreateBuffers(1, &vertex_buffer);
    glNamedBufferData(vertex_buffer, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int indices_buffer;
    glCreateBuffers(1, &indices_buffer);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
    glNamedBufferData(indices_buffer, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int vertex_array;
    glCreateVertexArrays(1, &vertex_array);

    // attach buffer
    glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(Vertex));

    // configure vertex attribute
    glEnableVertexArrayAttrib(vertex_array, 0);
    glVertexArrayAttribFormat(vertex_array, 0, 3, GL_FLOAT, false, 0);
    glVertexArrayAttribBinding(vertex_array, 0, 0);

    glVertexArrayElementBuffer(vertex_array, indices_buffer);

    glClearColor(0.8f, 0.2f, 0.1f, 0.0f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);
        glBindVertexArray(vertex_array);
        // glDrawArrays(GL_TRIANGLES, 0, 6); it use too much memory!
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &vertex_array);
    glDeleteBuffers(1, &vertex_buffer);

    glfwTerminate();
    return 0;
}

GLFWwindow *initialize_window() {
    if (!glfwInit()) {
        std::cout << "failed to init glfw\n";
        std::terminate();
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    auto *window = glfwCreateWindow(
            window_size.x, window_size.y,
            "OpenGL example", nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "failed to create window\n";
        std::terminate();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        std::cout << "failed to initialize glad\n";
        std::terminate();
    }

    return window;
}

unsigned int make_program() {
    unsigned int program;

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader);

    int success;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
        std::cout << "failed to compile vertex shader\n";
        std::cout << '\n' << info_log << '\n';
        std::terminate();
    }

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
        std::cout << "failed to compile fragment shader\n";
        std::cout << '\n' << info_log << '\n';
        std::terminate();
    }

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(program, 512, nullptr, info_log);
        std::cout << "failed to link program\n";
        std::cout << '\n' << info_log << '\n';
        std::terminate();
    }

    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}