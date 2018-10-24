#pragma once
#include "bounding_box.h"
#include "intersection.h"
#include "ray.h"

class Intersectable {
   public:
    Intersectable() = default;
    virtual ~Intersectable() = default;

    virtual bool IntersectionWith(const Ray* ray, Intersection* out_intersection) = 0;
    virtual void GenerateBoundingBox() = 0;

    const BoundingBox& GetBoundingBox() {
        if (!is_bounding_box_initialized) {
            GenerateBoundingBox();
        }

        return bounding_box_;
    }

   protected:
    BoundingBox bounding_box_;
    bool is_bounding_box_initialized = false;
};
