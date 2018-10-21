#include "pch.h"
#include "directionable.h"

Directionable::Directionable() : Directionable(Vector3(0, 0, -1)) {}

Directionable::Directionable(const Vector3& direction) : direction_(direction) {}

Directionable::~Directionable() = default;

void Directionable::SetDirection(const Vector3& direction) {
    direction_ = direction.Normalize();
}

const Vector3& Directionable::GetDirection() const {
    return direction_;
}
