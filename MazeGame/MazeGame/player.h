#pragma once
#include <vector>
#include "camera.h"
#include "game_object.h"
#include "map.h"

class Player : public GameObject {
   public:
    Player(Camera* camera, Map* map);

    void Update() override;

   private:
    void SetTransformToCameraPosition();

    Camera* camera_;
    Map* map_;
    std::vector<glm::vec4> box_;
};
