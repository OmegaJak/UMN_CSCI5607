#include <SDL.h>
#include "constants.h"
#include "player.h"

Player::Player(Camera* camera) : GameObject() {
    camera_ = camera;
    if (camera == nullptr) {
        printf("Player was given null camera. Exiting...\n");
        exit(1);
    }

    float num = 0.25f;
    float half_height = 0.425f;
    box_ = std::vector<glm::vec4>(8);
    box_[0] = glm::vec4(-num, -num, -half_height, 1);
    box_[1] = glm::vec4(num, -num, -half_height, 1);
    box_[2] = glm::vec4(num, num, -half_height, 1);
    box_[3] = glm::vec4(-num, num, -half_height, 1);
    box_[4] = glm::vec4(-num, -num, half_height, 1);
    box_[5] = glm::vec4(num, -num, half_height, 1);
    box_[6] = glm::vec4(num, num, half_height, 1);
    box_[7] = glm::vec4(-num, num, half_height, 1);

    SetTransformToCameraPosition();
}

void Player::Update() {
    const Uint8* key_state = SDL_GetKeyboardState(NULL);
    if (key_state[SDL_SCANCODE_RIGHT]) {
        camera_->Rotate(0, CAMERA_ROTATION_SPEED);
    } else if (key_state[SDL_SCANCODE_LEFT]) {
        camera_->Rotate(0, -CAMERA_ROTATION_SPEED);
    }

    if (key_state[SDL_SCANCODE_W]) {
        camera_->Translate(0, 0, CAMERA_MOVE_SPEED);
    } else if (key_state[SDL_SCANCODE_S]) {
        camera_->Translate(0, 0, -CAMERA_MOVE_SPEED);
    }
    if (key_state[SDL_SCANCODE_D]) {
        camera_->Translate(CAMERA_MOVE_SPEED, 0, 0);
    } else if (key_state[SDL_SCANCODE_A]) {
        camera_->Translate(-CAMERA_MOVE_SPEED, 0, 0);
    }

    SetTransformToCameraPosition();

    printf("Player bounds: min: %f, %f, %f, max:: %f, %f, %f\n", bounding_box_->Min().x, bounding_box_->Min().y, bounding_box_->Min().z,
           bounding_box_->Max().x, bounding_box_->Max().y, bounding_box_->Max().z);
}

void Player::SetTransformToCameraPosition() {
    transform->ResetLocalTransform();
    transform->Translate(camera_->GetPosition());
    InitBoundingBox(box_);
    bounding_box_->transform->ClearParent();
}
