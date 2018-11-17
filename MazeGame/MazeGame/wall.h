#pragma once
#include "game_object.h"

class Wall : public GameObject {
   public:
    Wall(Model* model) : GameObject(model) {}
    ~Wall() = default;

    bool IsSolid() override {
        return true;
    }
};
