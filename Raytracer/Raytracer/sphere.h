#pragma once
#include "primitive.h"
class Sphere : public Primitive {
   public:
    Sphere();
    Sphere(const Vector3& position, const Material& material, double radius);
    ~Sphere();

    double IntersectionWith(const Ray& ray) override;

   private:
    double radius_;
};
