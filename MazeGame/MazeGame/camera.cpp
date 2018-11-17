#include <SDL.h>
#include <gtc/type_ptr.hpp>
#include "camera.h"
#include "constants.h"
#include "glad.h"
#include "gtx/rotate_vector.hpp"
#include "shader_manager.h"

Camera::Camera() {
    position_ = glm::vec3(3, 0, 0.5);
    look_at_ = glm::vec3(0, 0, 0.5);
    up_ = glm::vec3(0, 0, 1);
}

Camera::~Camera() {}

glm::mat4 Camera::Rotate(float vertical_rotation, float horizontal_rotation, float roll_rotation) {
    glm::mat4 rotation_matrix;
    if (abs(vertical_rotation) > ABSOLUTE_TOLERANCE) {  // Avoid the computations if we can
        glm::vec3 right = glm::cross(look_at_ - position_, up_);

        look_at_ = glm::rotate(look_at_ - position_, vertical_rotation, right) + position_;
        up_ = glm::rotate(up_, vertical_rotation, right);
        // TODO: modify rotation matrix here
    }

    if (abs(horizontal_rotation) > ABSOLUTE_TOLERANCE) {
        look_at_ = glm::rotate(look_at_ - position_, -horizontal_rotation, up_) + position_;
        rotation_matrix = glm::rotate(rotation_matrix, horizontal_rotation, up_);
    }

    if (abs(roll_rotation) > ABSOLUTE_TOLERANCE) {
        up_ = glm::rotate(up_, roll_rotation, look_at_ - position_);
        // TODO: modify rotation matrix here
    }

    return rotation_matrix;
}

glm::mat4 Camera::Translate(float right, float up, float forward) {
    glm::vec3 forward_vec = glm::normalize(look_at_ - position_);
    glm::vec3 right_vec = glm::normalize(glm::cross(forward_vec, up_));

    glm::vec3 translation = right * right_vec + up * up_ + forward * forward_vec;
    position_ += translation;
    look_at_ += translation;

    return glm::translate(translation);
}

void Camera::SetPosition(const glm::vec3& position) {
    position_ = position;
}

glm::vec3 Camera::GetPosition() const {
    return position_;
}

void Camera::SetLookAt(const glm::vec3& look_at_position) {
    look_at_ = look_at_position;
}

void Camera::SetUp(const glm::vec3& up) {
    up_ = up;
}

void Camera::Update() {
    /*const Uint8* key_state = SDL_GetKeyboardState(NULL);
    if (key_state[SDL_SCANCODE_UP]) {
        Rotate(CAMERA_ROTATION_SPEED, 0);
    } else if (key_state[SDL_SCANCODE_DOWN]) {
        Rotate(-CAMERA_ROTATION_SPEED, 0);
    }
    if (key_state[SDL_SCANCODE_RIGHT]) {
        Rotate(0, CAMERA_ROTATION_SPEED);
    } else if (key_state[SDL_SCANCODE_LEFT]) {
        Rotate(0, -CAMERA_ROTATION_SPEED);
    }
    if (key_state[SDL_SCANCODE_E]) {
        Rotate(0, 0, CAMERA_ROTATION_SPEED);
    } else if (key_state[SDL_SCANCODE_Q]) {
        Rotate(0, 0, -CAMERA_ROTATION_SPEED);
    }

    if (key_state[SDL_SCANCODE_W]) {
        Translate(0, 0, CAMERA_MOVE_SPEED);
    } else if (key_state[SDL_SCANCODE_S]) {
        Translate(0, 0, -CAMERA_MOVE_SPEED);
    }
    if (key_state[SDL_SCANCODE_D]) {
        Translate(CAMERA_MOVE_SPEED, 0, 0);
    } else if (key_state[SDL_SCANCODE_A]) {
        Translate(-CAMERA_MOVE_SPEED, 0, 0);
    }
    if (key_state[SDL_SCANCODE_R]) {
        Translate(0, CAMERA_MOVE_SPEED, 0);
    } else if (key_state[SDL_SCANCODE_F]) {
        Translate(0, -CAMERA_MOVE_SPEED, 0);
    }*/

    glm::mat4 view = glm::lookAt(position_, look_at_, up_);
    // glm::mat4 inv = glm::inverse(view);
    // printf("CameraPos: %f, %f, %f\n", position_.x, position_.y, position_.z);
    glUniformMatrix4fv(ShaderManager::Attributes.view, 1, GL_FALSE, glm::value_ptr(view));
}
