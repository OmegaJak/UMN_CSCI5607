#pragma once
#include <vector>
#include "camera.h"
#include "game_object.h"

class Player : public GameObject {
   public:
    Player(Camera* camera, Map* map);

    void Update() override;

    glm::vec3 GetKeyPosition();

   private:
    void RegenerateBoundingBox();

    Camera* camera_;
    std::vector<glm::vec4> box_;
};
