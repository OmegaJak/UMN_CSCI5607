#pragma once
#include "light.h"
#include "point_light.h"
#include "directionable.h"

class SpotLight : public PointLight, public Directionable {
   public:
    SpotLight(Color color, const Vector3& position, const Vector3& direction, double inner_angle_, double outer_angle_);
    ~SpotLight();

    LightRecord GetLightRecordAt(const Vector3& position) override;

   private:
    double inner_angle_;
    double outer_angle_;
};
