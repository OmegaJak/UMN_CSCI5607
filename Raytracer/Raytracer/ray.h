#pragma once
#include "vector3.h"
#include <ostream>

class Ray {
   public:
    Ray();
    Ray(Vector3 start_point, Vector3 direction);
    ~Ray();

    friend std::ostream& operator<<(std::ostream& os, const Ray& color);

    Vector3 Evaluate(double t) const;

    Vector3 start_point_;
    Vector3 direction_;
};
