#pragma once
#include "game_object.h"

class Key : public GameObject {
   public:
    Key(Model* model) : GameObject(model) {}
    ~Key() = default;
};
