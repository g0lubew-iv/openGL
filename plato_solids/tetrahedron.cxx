//
// Created by one_eyed_john on 12/03/23.
//

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <model.hpp>
#include <camera.hpp>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

const char *vertex_shader_source =
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec3 in_color;\n"
        "uniform mat4 mvp;\n"
        "out vec3 color;\n"
        "void main() {\n"
        "   gl_Position = mvp * vec4(position, 1);\n"
        "   color = in_color;\n"
        "}\0";

const char *fragment_shader_source =
        "#version 330 core\n"
        "in vec3 color;\n"
        "out vec4 frag_color;\n"
        "void main() {\n"
        "   frag_color = vec4(color, 1);\n"
        "}\n\0";

struct Vertex {
    glm::vec3 pos;
    glm::vec3 col;
};

const glm::ivec2 window_size = {900, 900};
const double fps = 50.;
double rate = 1. / fps;

GLFWwindow *initialize_window();

unsigned int make_program();

Vertex vertices[] = {
        {{-1.f, -1.f, 0.f}, {1, 0, 0}},
        {{0.0f, 1.f,  0.f}, {0, 1, 0}},
        {{1.f,  -1.f, 0.f}, {0, 0, 1}},
        {{0.f,  0.f,  1.f}, {0, 1, 0}},
};

unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 1,
        1, 2, 3,
};

int main() {
    auto window = initialize_window();

    unsigned int program = make_program();

    int n = 4; // Number of faces of platonic solid
    // "Tetrahedron (4); Hexahedron (6); Octahedron (8); Dodecahedron (12); Icosahedron (20);

    unsigned int vertex_buffer;
    glCreateBuffers(1, &vertex_buffer);
    glNamedBufferData(vertex_buffer, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int indices_buffer;
    glCreateBuffers(1, &indices_buffer);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
    glNamedBufferData(indices_buffer, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int vertex_array;
    glCreateVertexArrays(1, &vertex_array);

    glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(Vertex));
    glEnableVertexArrayAttrib(vertex_array, 0);
    glVertexArrayAttribFormat(vertex_array, 0, 3, GL_FLOAT, false, 0);
    glVertexArrayAttribBinding(vertex_array, 0, 0);

    glVertexArrayVertexBuffer(vertex_array, 1, vertex_buffer, sizeof(glm::vec3), sizeof(Vertex));
    glEnableVertexArrayAttrib(vertex_array, 1);
    glVertexArrayAttribFormat(vertex_array, 0, 3, GL_FLOAT, false, 0);
    glVertexArrayAttribBinding(vertex_array, 1, 1);

    glVertexArrayElementBuffer(vertex_array, indices_buffer);

    int mvp_loc = glGetUniformLocation(program, "mvp");

    auto model = Model();
    model.set_size({10, 10, 10});
    auto camera = Camera();
    auto projection = glm::perspective(
            glm::radians(45.f),
            static_cast<float>(window_size.x) / static_cast<float>(window_size.y),
            0.1f, 100.f);

    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
//    glEnable(GL_FRONT);
//    glFrontFace(GL_CW);
    glEnable(GL_MULTISAMPLE);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    double last{0}, current{0}, duration{0};

    while (!glfwWindowShouldClose(window)) {
        last = current;
        current = glfwGetTime();
        duration += current - last;

        auto time = static_cast<float>(glfwGetTime());
        camera.set_position(
                glm::vec3(std::sin(time), std::sin(time),
                          std::cos(time)) * glm::vec3(50, 50, 50));
        camera.look_at({0, 0, 0});

        while (duration > rate) {
            duration -= rate;

            glfwPollEvents();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram(program);
            glBindVertexArray(vertex_array);

            auto mvp = projection * camera.get_view() * model.get_model();
            glUniformMatrix4fv(mvp_loc, 1, false, glm::value_ptr(mvp));
            glDrawElements(GL_TRIANGLES, int(std::pow(n, 2)), GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);

            glfwSwapBuffers(window);
        }
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