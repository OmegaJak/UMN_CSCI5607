#pragma once
#include "light.h"
#include "scene_object.h"

class PointLight : public Light {
   public:
    PointLight(Color color, Vector3 position);
    ~PointLight();

    Color GetIlluminanceAt(Vector3 position) override;
};
