#include "pch.h"
#include "directional_light.h"

DirectionalLight::DirectionalLight(Color color, Vector3 direction) : Light(color), Directionable(direction) {}

DirectionalLight::~DirectionalLight() {}

LightRecord DirectionalLight::GetLightRecordAt(const Vector3& position) {
    return LightRecord{-1 * direction_, color_};
}
