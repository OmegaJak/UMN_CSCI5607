#pragma once
#include <vector>
#include "camera.h"
#include "game_object.h"
#include "key.h"

class Player : public GameObject {
   public:
    Player(Camera* camera, Map* map);

    void Update() override;

   private:
    void RegenerateBoundingBox();
    void InitializeKeyLocation(Key* key);

    Camera* camera_;
    std::vector<glm::vec4> box_;
    Key* held_key_;
};
