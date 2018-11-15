#pragma once

#define GLM_FORCE_RADIANS
#include "gtc/matrix_transform.hpp"

class Transformable {
   public:
    Transformable();
    virtual ~Transformable();

    void Reset();
    void Rotate(float radians, const glm::vec3& around);
    void Translate(glm::vec3 translate_by);
    void Scale(const glm::vec3& scale);

   protected:
    glm::mat4 transform_;
};
