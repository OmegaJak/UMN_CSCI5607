#pragma once
#include "game_object.h"
#include "player.h"

class Map;

class Key : public GameObject {
   public:
    explicit Key(Model* model, Map* map);
    ~Key() = default;

    void Update() override;

   private:
    Player* player_;
};
