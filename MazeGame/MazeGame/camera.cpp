#include <gtc/type_ptr.hpp>
#include "camera.h"
#include "glad.h"
#include "shader_manager.h"

Camera::Camera() {
    position_ = glm::vec3(3, 0, 0);
    look_at_ = glm::vec3(0, 0, 0);
    up_ = glm::vec3(0, 0, 1);
}

Camera::~Camera() {}

void Camera::SetPosition(const glm::vec3& position) {
    position_ = position;
}

void Camera::SetLookAt(const glm::vec3& look_at_position) {
    look_at_ = look_at_position;
}

void Camera::SetUp(const glm::vec3& up) {
    up_ = up;
}

void Camera::Update() {
    glm::mat4 view = glm::lookAt(position_, look_at_, up_);
    glUniformMatrix4fv(ShaderManager::Attributes.view, 1, GL_FALSE, glm::value_ptr(view));
}
