#pragma once
#include "light.h"
#include "directionable.h"

class DirectionalLight : public Light, public Directionable {
   public:
    DirectionalLight(Color color, Vector3 direction);
    ~DirectionalLight();

    LightRecord GetLightRecordAt(const Vector3& position) override;
};
