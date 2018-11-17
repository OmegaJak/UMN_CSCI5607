#pragma once
#include "game_object.h"

class Door : public GameObject {
   public:
    Door(Model* model) : GameObject(model) {}
    ~Door() = default;

    bool IsSolid() override {
        return true;
    }
};
