//
// Created by one_eyed_john on 03/03/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm//mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>
#include <fstream>
#include <filesystem>

std::string get_shader_source(const std::filesystem::path&);

struct Vertex {
    float x;
    float y;
    float z;
};

Vertex vertices[] = {
        {-0.5f, -0.5f, 0.f},
        {0.0f,  0.5f, 0.f},
        {0.5f,  -0.5f, 0.f},
};

const auto window_size = glm::ivec2(900, 600);

int main() {
    if (!glfwInit()) {
        std::cout << "failed to init glfw\n";
        std::abort();
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    auto* window = glfwCreateWindow(window_size.x, window_size.y, "OpenGL example", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "failed to create window\n";
        std::abort();
    }

    glViewport(0, 0, window_size.x, window_size.y);

    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        std::cout << "failed to initialize glad\n";
        std::abort();
    }

    unsigned int program;

    { // make program
        auto vertex_shader_source = get_shader_source("simple.vert");
        auto vertex_shader_source_data = vertex_shader_source.c_str();

        unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_source_data, nullptr);
        glCompileShader(vertex_shader);

        int success;
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char info_log[512];
            glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
            std::cout << "failed to compile vertex shader\n";
            std::cout << '\n' << info_log << '\n';
            std::abort();
        }

        auto fragment_shader_source = get_shader_source("simple.frag");
        auto fragment_shader_source_data = fragment_shader_source.c_str();

        unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_source_data, nullptr);
        glCompileShader(fragment_shader);

        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char info_log[512];
            glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
            std::cout << "failed to compile fragment shader\n";
            std::cout << '\n' << info_log << '\n';
            std::abort();
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
            std::abort();
        }

        glDetachShader(program, vertex_shader);
        glDetachShader(program, fragment_shader);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }
    glUseProgram(program);

    unsigned int vertex_buffer;
    glCreateBuffers(1, &vertex_buffer);
    glNamedBufferData(vertex_buffer, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int uniform_buffer;
    glGenBuffers(1, &uniform_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer);
    glNamedBufferData(uniform_buffer, 128, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffer);

    unsigned int uniform_block_loc = glGetUniformBlockIndex(program, "matrices");
    glUniformBlockBinding(program, uniform_block_loc, 0);

    unsigned int vertex_array;
    glCreateVertexArrays(1, &vertex_array);
    // attach buffer
    glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(Vertex));
    // configure vertex attributes
    glEnableVertexArrayAttrib(vertex_array, 0);
    glVertexArrayAttribFormat(vertex_array, 0, 2, GL_FLOAT, false, 0);
    glVertexArrayAttribBinding(vertex_array, 0, 0);

    glBindVertexArray(vertex_array);

    auto projection = glm::perspective(
            glm::radians(45.f),
            static_cast<float>(window_size.x) / static_cast<float>(window_size.y),
            0.1f, 100.f);

    auto view = glm::mat4(1.f);
    auto model = glm::mat4(1.f);

    glNamedBufferSubData(uniform_buffer, 0, sizeof(glm::mat4), glm::value_ptr(view));


    glClearColor(1.f, 1.f, 1.f, 1.f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
    }

    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteBuffers(1, &vertex_buffer);

    glfwTerminate();
    return 0;
}

std::string get_shader_source(const std::filesystem::path& name) {
    auto path = "./shaders" / name;
    auto file = std::ifstream(path);
    std::string line;
    std::string source;
    while (file.good()) {
        std::getline(file, line);
        source += line;
        source += '\n';
    }
    return source;
}