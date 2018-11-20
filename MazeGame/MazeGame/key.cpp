#include <SDL.h>
#include <cmath>
#include "constants.h"
#include "key.h"
#include "map.h"
#include "player.h"

Key::Key(Model* model, Map* map, char id) : GameObject(model, map) {
    id_ = id;
}

void Key::Update() {
    Door* door = map_->IntersectsDoorWithId(this, id_);
    if (holder_ != nullptr && door != nullptr) {
        door->GoAway();
        GoAway();
        holder_->RemoveKey();
        holder_ = nullptr;
    }

    if (holder_ == nullptr) {
        transform->Rotate(KEY_ROTATION_SPEED, glm::vec3(0, 1, 0));

        int time = SDL_GetTicks();
        transform->Translate(0, cos(time / 700.0f) / 1000, 0);
    }

    GameObject::Update();
}

void Key::GoAway() {
    transform->ClearParent();
    transform->ResetAndSetTranslation(glm::vec3(0, 0, -3));
}

void Key::SetHolder(Player* player) {
    holder_ = player;
}
