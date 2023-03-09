//
// Created by asuka on 08.03.2023.
//

# include "model.hpp"
# include <glm/gtc/matrix_transform.hpp>

Model::Model(glm::vec3 position, glm::vec3 rotation, glm::vec3 size)
        : position_(position), rotation_(rotation), size_(size) {}

glm::mat4 Model::get_model() {
    if (should_update_model_) {
        model_ = glm::mat4(1.f);
        model_ = glm::translate(model_, position_);
        model_ = glm::scale(model_, size_);
        model_ = glm::rotate(model_, rotation_.x, {1, 0, 0});
        model_ = glm::rotate(model_, rotation_.y, {0, 1, 0});
        model_ = glm::rotate(model_, rotation_.z, {0, 0, 1});
        should_update_model_ = false;
    }
    return model_;
}

void Model::translate(glm::vec3 vector) {
    position_ += vector;
    should_update_model_ = true;
}

void Model::rotate(glm::vec3 angeles) {
    rotation_ += angeles;
    should_update_model_ = true;
}

void Model::scale(glm::vec3 shape) {
    size_ *= shape;
    should_update_model_ = true;
}

void Model::set_position(glm::vec3 position) {
    position_ = position;
    should_update_model_ = true;
}

void Model::set_rotation(glm::vec3 rotation) {
    rotation_ = rotation;
    should_update_model_ = true;
}

void Model::set_size(glm::vec3 size) {
    size_ = size;
    should_update_model_ = true;
}

glm::vec3 Model::get_position() const {
    return position_;
}

glm::vec3 Model::get_rotation() const {
    return rotation_;
}

glm::vec3 Model::get_size() const {
    return size_;
}
