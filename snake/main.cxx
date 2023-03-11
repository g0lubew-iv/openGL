//
// Created by one_eyed_john on 09/03/23.
//

#include <glfw_system.hxx>
#include <window.hxx>

int main() {
    auto glfw = snake::GLFWSystem();
    auto window = snake::Window({{900, 600}, "Snake"});

    /*while (!window.should_close()) {
        glfw.pool_events();
        window.swap_buffers();
    }*/

    return 0;
}