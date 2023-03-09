//
// Created by one_eyed_john on 09/03/23.
//

#ifndef OPENGL_EXAMPLES_GLFW_SYSTEM_HXX
#define OPENGL_EXAMPLES_GLFW_SYSTEM_HXX

namespace snake {

    class GLFWSystem {
        public:
            GLFWSystem();
            ~GLFWSystem();

            GLFWSystem(const GLFWSystem&) = delete;
            GLFWSystem& operator=(const GLFWSystem&) = delete;

            GLFWSystem(GLFWSystem&&) noexcept = delete;
            GLFWSystem& operator=(GLFWSystem&&) noexcept = delete;

            static bool is_initialized() noexcept;

        private:
            static inline bool is_initialized_ = false;
    };

}

#endif //OPENGL_EXAMPLES_GLFW_SYSTEM_HXX
