#pragma once
#include "game_object.h"

class Goal : public GameObject {
   public:
    Goal(Model* model) : GameObject(model) {}
    ~Goal() = default;
};
