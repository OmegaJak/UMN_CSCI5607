#include "pch.h"
#include "ambient_light.h"

AmbientLight::AmbientLight() {}

AmbientLight::~AmbientLight() = default;

AmbientLight::AmbientLight(Color color) : color_(color) {}

Color AmbientLight::GetColor() const {
    return color_;
}
