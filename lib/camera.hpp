//
// Created by asuka on 08.03.2023.
//

# ifndef OPENGL_EXAMPLE_SPACE_CAMERA_HPP
# define OPENGL_EXAMPLE_SPACE_CAMERA_HPP

# include <glm/vec3.hpp>
# include <glm/mat4x4.hpp>

class Camera {
public:
    Camera() = default;
    Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 world_up);

    [[nodiscard]] glm::mat4 get_view();

    void look_at(glm::vec3 target);
    void translate(glm::vec3 vector);
    void rotate(glm::vec2 angeles);

    void set_position(glm::vec3 position);
    void set_forward(glm::vec3 forward);
    void set_world_up(glm::vec3 world_up);

    [[nodiscard]] glm::vec3 get_position() const;
    [[nodiscard]] glm::vec3 get_forward() const;
    [[nodiscard]] glm::vec3 get_world_up() const;

private:
    bool should_update_view_ = true;
    glm::mat4 view_{1.f};

    glm::vec3 position_ = {0, 0, 0};
    glm::vec3 forward_ = {0, 0, -1};
    glm::vec3 world_up_ = {0, 1, 0};
};

# endif // OPENGL_EXAMPLE_SPACE_CAMERA_HPP
