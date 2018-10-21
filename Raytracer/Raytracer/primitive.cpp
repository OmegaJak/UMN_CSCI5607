#include "pch.h"
#include "primitive.h"

Primitive::Primitive() {}

Primitive::Primitive(Material material) : material_(material) {}

Primitive::~Primitive() = default;

void Primitive::SetMaterial(const Material& material) {
    material_ = material;
}

const Material& Primitive::GetMaterial() const {
    return material_;
}

