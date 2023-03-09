//
// Created by asuka on 03.03.2023.
//

# include <glad/glad.h>
# include <GLFW/glfw3.h>

# include <glm/vec2.hpp>
# include <glm/mat4x4.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>

# include <iostream>
# include <fstream>
# include <filesystem>

# include <model.hpp>
# include <camera.hpp>

struct Vertex {
    float x;
    float y;
    float z;
};

Vertex vertices[] = {
        {-0.5f, -0.433f, 0.f},
        {0.f,   0.433f,  0.f},
        {0.5f,  -0.433f, 0.f},
};

const glm::ivec2 window_size = {900, 600};

std::string get_shader_source(const std::filesystem::path &name);

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

    auto *window = glfwCreateWindow(
            window_size.x, window_size.y, "OpenGL example", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "failed to create window\n";
        std::abort();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        std::cout << "failed to initialize glad\n";
        std::abort();
    }

    glViewport(0, 0, window_size.x, window_size.y);

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

    unsigned int vertex_array;
    glCreateVertexArrays(1, &vertex_array);
    // attach buffer
    glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(Vertex));
    // configure vertex attributes
    glEnableVertexArrayAttrib(vertex_array, 0);
    glVertexArrayAttribFormat(vertex_array, 0, 3, GL_FLOAT, false, 0);
    glVertexArrayAttribBinding(vertex_array, 0, 0);

    glBindVertexArray(vertex_array);
    int mvp_loc = glGetUniformLocation(program, "mvp");

    auto model = Model();
    model.set_size({20, 20, 1});
    auto camera = Camera();
    auto projection = glm::perspective(
            glm::radians(45.f),
            static_cast<float>(window_size.x) / static_cast<float>(window_size.y),
            0.1f, 100.f);

    glClearColor(1.f, 1.f, 1.f, 1.f);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        auto time = static_cast<float>(glfwGetTime());
        auto value = std::sin(time);
        model.set_position(glm::vec3(0, 0, 20) * value);
        model.rotate({0, 0, value * 0.003});
        camera.set_position(glm::vec3(std::sin(time), std::sin(time), std::cos(time)) * glm::vec3(50, 20, 50));
        camera.look_at({0, 0, 0});
        auto mvp = projection * camera.get_view() * model.get_model();
        glUniformMatrix4fv(mvp_loc, 1, false, glm::value_ptr(mvp));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
    }

    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteBuffers(1, &vertex_buffer);

    glfwTerminate();
    return 0;
}

std::string get_shader_source(const std::filesystem::path &name) {
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
