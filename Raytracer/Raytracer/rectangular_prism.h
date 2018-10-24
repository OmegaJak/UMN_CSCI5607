#pragma once
#include "primitive.h"
#include "intersectable_group.h"

class RectangularPrism : public Primitive {
   public:
    RectangularPrism(const Vector3& point, const Vector3& right, const Vector3& up, const Vector3& forward);
    ~RectangularPrism();

    bool IntersectionWith(const Ray* ray, Intersection* out_intersection) override;
    void GenerateBoundingBox() override;

   private:
    Vector3 point_, right_, up_, forward_;
    IntersectableGroup faces_;
};
