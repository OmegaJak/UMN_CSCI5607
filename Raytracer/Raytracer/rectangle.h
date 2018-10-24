#pragma once
#include "plane.h"
class Rectangle : public Plane {
   public:
    Rectangle(const Vector3& point, const Vector3& right, const Vector3& up, bool two_sided = true);
    ~Rectangle();

    bool IntersectionWith(const Ray* ray, Intersection* out_intersection) override;
    void GenerateBoundingBox() override;

   private:
    Vector3 right_, up_, normed_right_, normed_up_;
    double right_length_, up_length_;
};
