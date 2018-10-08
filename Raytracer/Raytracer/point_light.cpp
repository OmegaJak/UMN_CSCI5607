#include "pch.h"
#include "point_light.h"

PointLight::PointLight(Color color, Vector3 position) : Light(color, position) {}

PointLight::~PointLight() {}

Color PointLight::GetIlluminanceAt(Vector3 position) {
    double distance = (position_ - position).Magnitude();
    return (color_ * (1 / (distance * distance))).Clamp();
}
