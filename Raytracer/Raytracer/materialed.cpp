#include "pch.h"
#include "materialed.h"

Materialed::Materialed() : Materialed(Material()) {}

Materialed::Materialed(const Material& material) : material_(material) {}

Materialed::~Materialed() = default;

void Materialed::SetMaterial(const Material& material) {
    material_ = material;
}

const Material& Materialed::GetMaterial() const {
    return material_;
}
