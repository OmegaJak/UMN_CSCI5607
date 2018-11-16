#pragma once
#define GLM_FORCE_RADIANS
#include <gtc/matrix_transform.hpp>
#include "updatable.h"

class Camera : public Updatable {
   public:
    Camera();
    ~Camera() override;

    void Rotate(float vertical_rotation, float horizontal_rotation, float roll_rotation = 0);
    void Translate(float right, float up, float forward);

    void SetPosition(const glm::vec3& position);
    void SetLookAt(const glm::vec3& look_at_position);
    void SetUp(const glm::vec3& up);

    void Update() override;

   private:
    glm::vec3 position_, look_at_, up_;
};
