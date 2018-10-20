#pragma once
#include "light.h"
#include "point_light.h"

class SpotLight : public PointLight {
   public:
    SpotLight(Color color, const Vector3& position, const Vector3& direction, double inner_angle_, double outer_angle_);
    ~SpotLight();

    Color GetIlluminanceAt(const Vector3& position) override;

   private:
    Vector3 direction_;
    double inner_angle_;
    double outer_angle_;
};
