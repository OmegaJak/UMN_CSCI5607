#define GLM_FORCE_RADIANS

#include <SDL.h>
#include "constants.h"
#include "map.h"
#include "player.h"

Player::Player(Camera* camera, Map* map) : GameObject() {
    camera_ = camera;
    if (camera == nullptr) {
        printf("Player was given null camera. Exiting...\n");
        exit(1);
    }

    map_ = map;
    if (map == nullptr) {
        printf("Player was given null map. Exiting...\n");
        exit(1);
    }

    float num = 0.25f;
    float half_height = 0.25f;
    box_ = std::vector<glm::vec4>(8);
    box_[0] = glm::vec4(-num, -num, -half_height, 1);
    box_[1] = glm::vec4(num, -num, -half_height, 1);
    box_[2] = glm::vec4(num, num, -half_height, 1);
    box_[3] = glm::vec4(-num, num, -half_height, 1);
    box_[4] = glm::vec4(-num, -num, half_height, 1);
    box_[5] = glm::vec4(num, -num, half_height, 1);
    box_[6] = glm::vec4(num, num, half_height, 1);
    box_[7] = glm::vec4(-num, num, half_height, 1);

    glm::vec3 start_position = map_->SpawnPosition();
    start_position.z = 2 * half_height;
    camera->SetPosition(start_position);

    glm::vec3 look_position = map_->GoalPosition();
    look_position.z = start_position.z;
    camera->SetLookAt(look_position);

    transform->SetParent(camera->transform);
    RegenerateBoundingBox();

    held_key_ = nullptr;
}

void Player::Update() {
    //// Player movement ////
    const Uint8* key_state = SDL_GetKeyboardState(NULL);
    if (key_state[SDL_SCANCODE_RIGHT]) {
        camera_->Rotate(0, -CAMERA_ROTATION_SPEED);
    } else if (key_state[SDL_SCANCODE_LEFT]) {
        camera_->Rotate(0, CAMERA_ROTATION_SPEED);
    }

    float movement_forward = 0, movement_right = 0;
    if (key_state[SDL_SCANCODE_W]) {
        movement_forward += CAMERA_MOVE_SPEED;  // movement forward
    } else if (key_state[SDL_SCANCODE_S]) {
        movement_forward -= CAMERA_MOVE_SPEED;
    }
    if (key_state[SDL_SCANCODE_D]) {
        movement_right += CAMERA_MOVE_SPEED;
    } else if (key_state[SDL_SCANCODE_A]) {
        movement_right -= CAMERA_MOVE_SPEED;
    }
    camera_->Translate(movement_right, 0, movement_forward);

    RegenerateBoundingBox();

    if (map_->IntersectsAnySolidObjects(this)) {
        camera_->Translate(-movement_right, 0, -movement_forward);  // Reverse the camera movement
        RegenerateBoundingBox();
    }

    printf("Player bounds: min: %f, %f, %f, max:: %f, %f, %f\n", bounding_box_->Min().x, bounding_box_->Min().y, bounding_box_->Min().z,
           bounding_box_->Max().x, bounding_box_->Max().y, bounding_box_->Max().z);

    //// Key logic ////
    Key* key = map_->FirstIntersectedKey(this);
    if (key != nullptr && held_key_ == nullptr) {
        held_key_ = key;
        InitializeKeyLocation(held_key_);
    }
}

void Player::InitializeKeyLocation(Key* key) {
    held_key_->transform->ResetAndSetTranslation(camera_->GetNormalizedLookPosition() * 0.6f);
    held_key_->transform->Translate(0, 0, -0.2);
    held_key_->transform->Scale(0.3f);

    held_key_->transform->SetParent(transform);
}

void Player::RegenerateBoundingBox() {
    InitBoundingBox(box_);
    bounding_box_->transform->ClearParent();
}
