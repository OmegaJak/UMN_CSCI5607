#include "pch.h"
#include "point_light.h"

PointLight::PointLight(Color color, Vector3 position) : Light(color, position) {}

PointLight::~PointLight() {}

Color PointLight::GetIlluminanceAt(const Vector3& position) {
    double sqr_distance = (position_ - position).SqrMagnitude();
    return (color_ * (1 / sqr_distance)).Clamp();
}
