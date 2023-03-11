//
// Created by one_eyed_john on 09/03/23.
//

#ifndef OPENGL_EXAMPLES_WINDOW_HXX
#define OPENGL_EXAMPLES_WINDOW_HXX

#include <glm/vec2.hpp>
#include <string>

struct GLFWwindow;  // GLFW/glfw3.h

namespace snake {

    struct DisplayData {
        glm::ivec2 size;
        std::string title;
    };

    class Window {
    public:
        explicit Window(const DisplayData &display_data);
        ~Window();

        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;

        Window(Window &&other) noexcept;
        Window &operator=(Window &&other) noexcept;

    private:
        GLFWwindow *window = nullptr;
    };

}

#endif //OPENGL_EXAMPLES_WINDOW_HXX
