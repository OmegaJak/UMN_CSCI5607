#include "pch.h"
#include "point_light.h"

PointLight::PointLight(Color color, Vector3 position) : Light(color), Positionable(position) {}

PointLight::~PointLight() = default;

LightRecord PointLight::GetLightRecordAt(const Vector3& position) {
    Vector3 to_light = GetPosition() - position;
    double sqr_distance = to_light.SqrMagnitude();
    Color color = color_ * (1 / sqr_distance);
    return LightRecord{to_light, color};
}
