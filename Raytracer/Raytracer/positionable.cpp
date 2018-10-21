#include "pch.h"
#include "positionable.h"

Positionable::Positionable() : Positionable(Vector3(0, 0, 0)) {}

Positionable::Positionable(const Vector3& position) : position_(position) {}

void Positionable::SetPosition(const Vector3& position) {
    position_ = position;
}

const Vector3& Positionable::GetPosition() {
    return position_;
}

