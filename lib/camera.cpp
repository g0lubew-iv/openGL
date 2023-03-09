//
// Created by asuka on 08.03.2023.
//

# include "camera.hpp"
# include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 world_up)
        : position_(position), forward_(forward), world_up_(world_up) {}

glm::mat4 Camera::get_view() {
    if (should_update_view_) {
        view_ = glm::lookAt(position_, position_ + forward_, world_up_);
        should_update_view_ = false;
    }
    return view_;
}

void Camera::look_at(glm::vec3 target) {
    forward_ = target - position_;
    should_update_view_ = true;
}

void Camera::translate(glm::vec3 vector) {
    position_ += vector;
    should_update_view_ = true;
}

void Camera::rotate(glm::vec2 angeles) {
    // TODO:
}

void Camera::set_position(glm::vec3 position) {
    position_ = position;
    should_update_view_ = true;
}

void Camera::set_forward(glm::vec3 forward) {
    forward_ = forward;
    should_update_view_ = true;
}

void Camera::set_world_up(glm::vec3 world_up) {
    world_up_ = world_up;
    should_update_view_ = true;
}

glm::vec3 Camera::get_position() const {
    return position_;
}

glm::vec3 Camera::get_forward() const {
    return forward_;
}

glm::vec3 Camera::get_world_up() const {
    return world_up_;
}
