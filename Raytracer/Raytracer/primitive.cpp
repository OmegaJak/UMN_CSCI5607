#include "pch.h"
#include "primitive.h"

Primitive::Primitive() {
    position_ = Vector3();
}

Primitive::Primitive(Vector3 position, Material material) : SceneObject(position), material_(material) {}

Primitive::~Primitive() = default;
