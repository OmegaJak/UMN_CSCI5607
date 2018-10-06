#pragma once
#include "primitive.h"
class Sphere : public Primitive {
   public:
    Sphere();
    Sphere(const Vector3& position, const Material& material, double radius);
    ~Sphere();

    bool IntersectionWith(const Ray& ray, Intersection& out_intersection) override;

   private:
    double radius_;
};
