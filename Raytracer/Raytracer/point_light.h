#pragma once
#include "positionable.h"
#include "light.h"

class PointLight : public Light, public Positionable {
   public:
    PointLight(Color color, Vector3 position);
    ~PointLight();

    LightRecord GetLightRecordAt(const Vector3& position) override;
};
