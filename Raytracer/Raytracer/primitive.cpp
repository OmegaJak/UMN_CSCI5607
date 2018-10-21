#include "pch.h"
#include "primitive.h"

Primitive::Primitive() : Positionable(Vector3()) {}

Primitive::Primitive(Vector3 position, Material material) : Positionable(position), material_(material) {}

Primitive::~Primitive() = default;
