#pragma once
#include "primitive.h"
#include "positionable.h"

class Sphere : public Primitive, public Positionable {
   public:
    Sphere();
    Sphere(const Vector3& position, double radius);
    ~Sphere();

    bool IntersectionWith(const Ray* ray, Intersection* out_intersection) override;
    void GenerateBoundingBox() override;

   private:
    double radius_;
    double r_squared_;
};
