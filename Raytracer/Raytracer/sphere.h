#pragma once
#include "primitive.h"
class Sphere : public Primitive {
   public:
    Sphere();
    Sphere(Vector3 position, Material material, double radius);
    ~Sphere();

   private:
    double radius_;
};
