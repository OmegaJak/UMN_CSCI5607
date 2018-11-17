#include "key.h"
#include "map.h"
#include "player.h"

Key::Key(Model* model, Map* map) : GameObject(model, map) {}

void Key::Update() {
    Player* player = nullptr;
    player = map_->IntersectsPlayer(this);
    if (player && !player_) player_ = player;  // The player just walked into this key

    if (player_) {
        transform->ResetLocalTransform();
        transform->Translate(player_->GetKeyPosition());

        if (!transform->IsParent(player_->transform)) transform->SetParent(player_->transform);
    }

    printf("Key position: %f, %f, %f\n", transform->X(), transform->Y(), transform->Z());

    GameObject::Update();
}
