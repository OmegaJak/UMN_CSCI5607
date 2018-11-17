#pragma once
#include "camera.h"
#include "game_object.h"

class Player : public GameObject {
   public:
    explicit Player(Camera* camera);

    void Update() override;

   private:
    void SetTransformToCameraPosition();

    Camera* camera_;
    std::vector<glm::vec4> box_;
};
