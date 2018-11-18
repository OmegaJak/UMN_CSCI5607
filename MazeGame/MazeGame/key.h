#pragma once
#include "game_object.h"

class Map;

class Key : public GameObject {
   public:
    explicit Key(Model* model, Map* map);
    ~Key() = default;
};
