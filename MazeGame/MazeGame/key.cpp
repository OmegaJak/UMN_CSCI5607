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

    GameObject::Update();
}

void Key::GoAway() {
    transform->ClearParent();
    transform->ResetAndSetTranslation(glm::vec3(0, 0, -3));
}

void Key::SetHolder(Player* player) {
    holder_ = player;
}
