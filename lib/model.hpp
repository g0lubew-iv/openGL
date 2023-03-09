//
// Created by asuka on 08.03.2023.
//

# ifndef OPENGL_EXAMPLE_MODEL_HPP
# define OPENGL_EXAMPLE_MODEL_HPP

# include <glm/vec3.hpp>
# include <glm/mat4x4.hpp>

class Model {
public:
    Model() = default;
    Model(glm::vec3 position, glm::vec3 rotation, glm::vec3 size);

    [[nodiscard]] glm::mat4 get_model();

    void translate(glm::vec3 vector);
    void rotate(glm::vec3 angeles);
    void scale(glm::vec3 shape);

    void set_position(glm::vec3 position);
    void set_rotation(glm::vec3 rotation);
    void set_size(glm::vec3 size);

    [[nodiscard]] glm::vec3 get_position() const;
    [[nodiscard]] glm::vec3 get_rotation() const;
    [[nodiscard]] glm::vec3 get_size() const;

private:
    bool should_update_model_ = true;
    glm::mat4 model_{1.f};

    glm::vec3 position_ = {0, 0, 0};
    glm::vec3 rotation_ = {0, 0, 0};
    glm::vec3 size_ = {1, 1, 1};
};

# endif // OPENGL_EXAMPLE_MODEL_HPP
