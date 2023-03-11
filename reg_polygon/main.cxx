//
// Created by one_eyed_john on 11/03/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include <iostream>
#include <cmath>

const char *vertex_shader_source =
        "#version 330 core\n"
        "layout (location = 0) in vec2 position;\n"
        "void main() {\n"
        "   gl_Position = vec4(position, 0, 1);\n"
        "}\0";

const char *fragment_shader_source =
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main() {\n"
        "   color = vec4(0, 0.8, 0.5, 1);\n"
        "}\n\0";

struct Vertex {
    float x;
    float y;
};

/*float calc_distortion(int a, int b) {
    return a > b ? static_cast<float>(a) / static_cast<float>(b) : static_cast<float>(b) / static_cast<float>(a);
}*/
// TODO: перенести в сам расчёт многоугольника - после

constexpr float pi() { return static_cast<float>(std::atan(1)) * 4.; }
const float radius = 1.f;
const glm::ivec2 window_size = {900, 900};
// const float distortion = calc_distortion(window_size.x, window_size.y);
const char* title = "Regular Polygon";

GLFWwindow *initialize_window();

unsigned int make_program();

int main() {

    int n = 3; // number of vertices
    std::cin >> n;

    if (n <= 2) {
        std::cout << "Incorrect number of vertices!\n";
        std::terminate();
    }

    Vertex vertices[n];
    unsigned int indices[3 * (n - 2)];

    const auto angle = pi() * static_cast<float>((n - 2)) / static_cast<float>(n);
    unsigned int counter = 1;
    unsigned int index = 0;

    for (int i = 0; i < n; i++) {

        vertices[i] = Vertex{radius * float(std::sin(pi() / float(n)) * std::cos((pi() - angle) * float(i - 1))),
                             radius * float(std::sin(pi() / float(n)) * std::sin((pi() - angle) * float(i - 1))),
        };

        if (i < (n - 2)) {
            counter--;
            indices[index] = 0;
            indices[index + 1] = ++counter;
            indices[index + 2] = ++counter;
            index += 3;
        }
    }

    /*for (int j = 0; j < 3 * (n - 2); j++) {
        std::cout << indices[j] << " ";
    }*/

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
    glVertexArrayAttribFormat(vertex_array, 0, 2, GL_FLOAT, false, 0);
    glVertexArrayAttribBinding(vertex_array, 0, 0);

    glVertexArrayElementBuffer(vertex_array, indices_buffer);

    glClearColor(0.f, 0.f, 0.f, 0.f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);
        glBindVertexArray(vertex_array);
        // glDrawArrays(GL_TRIANGLES, 0, 6); it use too much memory!
        glDrawElements(GL_TRIANGLES, n * 3, GL_UNSIGNED_INT, nullptr);
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

    auto *window = glfwCreateWindow(window_size.x, window_size.y, title,
                                    nullptr, nullptr);
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
        std::cout << "Failed to link program!\n";
        std::cout << '\n' << info_log << '\n';
        std::terminate();
    }

    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}