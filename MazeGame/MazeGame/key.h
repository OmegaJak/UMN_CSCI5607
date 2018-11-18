#pragma once
#include "game_object.h"

class Map;
class Player;

class Key : public GameObject {
   public:
    explicit Key(Model* model, Map* map, char id);
    ~Key() = default;

    void Update() override;
    void GoAway();
    void SetHolder(Player* player);

   private:
    char id_;
    Player* holder_;
};
