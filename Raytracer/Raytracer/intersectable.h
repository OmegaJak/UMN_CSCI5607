#pragma once
#include "ray.h"
#include "intersection.h"

class Intersectable {
   public:
    Intersectable() = default;
    virtual ~Intersectable() = default;

    virtual bool IntersectionWith(const Ray* ray, Intersection* out_intersection) = 0;
};
