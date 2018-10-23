#include "pch.h"
#include "rectangular_prism.h"
#include "Rectangle.h"

RectangularPrism::RectangularPrism(const Vector3& point, const Vector3& right, const Vector3& up, const Vector3& forward)
    : point_(point), right_(right), up_(up), forward_(forward) {
    if (up_.Dot(right_) != 0 || right_.Dot(forward_) != 0 || forward_.Dot(up_) != 0) {
        throw "A rectangular prism's sides must be perpendicular to one another!";
    }

    auto front = new Rectangle(point, right_, up_, false);
    auto left = new Rectangle(point + forward_, -1 * forward, up_, false);
    auto bottom = new Rectangle(point + forward, right_, -1 * forward, false);
    auto right_face = new Rectangle(point + right_, forward_, up_, false);
    auto top = new Rectangle(point + up_, right_, forward_, false);
    auto back = new Rectangle(point + right_ + forward_, -1 * right_, up_, false);

    faces_.AddPrimitive(front);
    faces_.AddPrimitive(left);
    faces_.AddPrimitive(bottom);
    faces_.AddPrimitive(right_face);
    faces_.AddPrimitive(top);
    faces_.AddPrimitive(back);
}

RectangularPrism::~RectangularPrism() = default;

bool RectangularPrism::IntersectionWith(const Ray* ray, Intersection* out_intersection) {
    bool intersected = faces_.IntersectionWith(ray, out_intersection);
    if (intersected) {
        out_intersection->object_ = this;
    }

    return intersected;
}
