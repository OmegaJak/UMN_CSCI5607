#pragma once
#include "vector3.h"

class Directionable {
   public:
    Directionable();
    explicit Directionable(const Vector3& direction);
    ~Directionable();

    void SetDirection(const Vector3& direction);
    const Vector3& GetDirection() const;

   protected:
    Vector3 direction_;
};
