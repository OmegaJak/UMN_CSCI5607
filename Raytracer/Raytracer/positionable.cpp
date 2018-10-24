#include "pch.h"
#include "positionable.h"
#include "time_keeper.h"

Positionable::Positionable() : Positionable(Vector3(0, 0, 0)) {}

Positionable::Positionable(const Vector3& position) : position_(position) {}

void Positionable::SetPosition(const Vector3& position) {
    position_ = position;
}

const Vector3& Positionable::GetPosition() {
    if (is_moving_ && TimeKeeper::GetTime() != last_time_) {
        last_time_ = TimeKeeper::GetTime();
        position_ = start_position_ + last_time_ * velocity_;
    }

    return position_;
}

void Positionable::SetEndPosition(const Vector3& end_position) {
    is_moving_ = true;
    start_position_ = position_;
    end_position_ = end_position;
    velocity_ = end_position_ - start_position_;
}

bool Positionable::IsMoving() const {
    return is_moving_;
}
