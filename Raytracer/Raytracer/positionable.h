#pragma once
#include "vector3.h"

class Positionable {
   public:
    Positionable();
    explicit Positionable(const Vector3& position);

    virtual void SetPosition(const Vector3& position);
    virtual const Vector3& GetPosition();

    virtual void SetEndPosition(const Vector3& end_position);
    bool IsMoving() const;

   private:
    Vector3 position_;
    bool is_moving_ = false;
    Vector3 start_position_, end_position_, velocity_;
    double last_time_ = 0.0;
};
