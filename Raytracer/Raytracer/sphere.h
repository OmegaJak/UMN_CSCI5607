#pragma once
#include "primitive.h"
class Sphere : public Primitive {
   public:
    Sphere();
    Sphere(const Vector3& position, const Material& material, double radius);
    ~Sphere();

   private:
    double radius_;
};
