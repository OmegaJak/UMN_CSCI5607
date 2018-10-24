#pragma once
#include "ray.h"
#include "intersection.h"

class Intersectable {
   public:
    Intersectable();
    virtual ~Intersectable();

    virtual bool IntersectionWith(const Ray* ray, Intersection* out_intersection) = 0;
};
