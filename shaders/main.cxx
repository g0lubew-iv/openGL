//
// Created by one_eyed_john on 03/03/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <gl_functions/gl_functions.hpp>

#include <iostream>

const char* vertex_shader_source =
        "#version 330 core\n"
        "layout (location = 0) in vec2 position;\n"
        "void main() {\n"
        "   gl_Position = vec4(position, 0, 1);\n"
        "}\0";

const char* fragment_shader_source =
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main() {\n"
        "   color = vec4(1, 0.8, 0.1, 1);\n"
        "}\n\0";

struct Vertex {
    float x;
    float y;
};

Vertex vertices[] = {
        {-0.5f, -0.5f},
        {0.0f, 0.5f},
        {0.5f, -0.5f},
};

int main() {
    auto window = initialize_window();

    unsigned int program = make_program();

    unsigned int vertex_buffer;
    glCreateBuffers(1, &vertex_buffer);
    glNamedBufferData(vertex_buffer, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int vertex_array;
    glCreateVertexArrays(1, &vertex_array);

    // attach buffer
    glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(Vertex));

    // configure vertex attribute
    glEnableVertexArrayAttrib(vertex_array, 0);
    glVertexArrayAttribFormat(vertex_array, 0, 2, GL_FLOAT, false, 0);
    glVertexArrayAttribBinding(vertex_array, 0, 0);

    glClearColor(0.8f, 0.2f, 0.1f, 0.0f);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);
        glBindVertexArray(vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &vertex_array);
    glDeleteBuffers(1, &vertex_buffer);

    glfwTerminate();
    return 0;
}