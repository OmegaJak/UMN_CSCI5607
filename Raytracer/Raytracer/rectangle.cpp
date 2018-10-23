#include "pch.h"
#include "Rectangle.h"

Rectangle::Rectangle(const Vector3& point, const Vector3& right, const Vector3& up, bool two_sided)
    : right_(right), up_(up) {
    if (right.Dot(up) != 0) {
        throw "Rectangle must have perpendicular sides!";
    }

    two_sided_ = two_sided;
    normal_ = right_.Cross(up_).Normalize();
    point_ = point;
    normed_right_ = right_.Normalize();
    normed_up_ = up_.Normalize();
    up_length_ = up_.Magnitude();
    right_length_ = right_.Magnitude();
}

Rectangle::~Rectangle() = default;

bool Rectangle::IntersectionWith(const Ray* ray, Intersection* out_intersection) {
    static Intersection plane_intersection;
    plane_intersection.ResetT();

    if (Plane::IntersectionWith(ray, &plane_intersection)) {
        Vector3 to_hit_point = plane_intersection.hit_point_ - point_;

        double u = to_hit_point.Dot(normed_right_);
        if (u <= 0 || u >= right_length_) return false;

        double v = to_hit_point.Dot(normed_up_);
        if (v <= 0 || v >= up_length_) return false;

        if (out_intersection->ConsiderIntersection(plane_intersection)) {
            out_intersection->object_ = this;

            return true;
        }
    }

    return false;
}
