#include "pch.h"
#include "sphere.h"
#include <cstdio>
#include <algorithm>

Sphere::Sphere() {}

Sphere::Sphere(const Vector3& position, const Material& material, const double radius) : Primitive(position, material)
{
    radius_ = radius;
#ifdef _DEBUG
    printf("Sphere created at (%f, %f, %f) with radius %f.\n", position.GetX(), position.GetY(), position.GetZ(), radius);
#endif
}

Sphere::~Sphere() = default;

bool Sphere::IntersectionWith(const Ray& ray, Intersection& out_intersection) {
    double a = ray.direction_.SqrMagnitude();
    double b = (2 * ray.direction_).Dot(ray.start_point_ - position_);
    double c = (ray.start_point_ - position_).SqrMagnitude() - (radius_ * radius_);

    double discriminant = (b * b) - 4 * a * c;
    double t1 = std::max((-b + sqrt(discriminant)) / (2 * a), 0.0);
    double t2 = std::max((-b - sqrt(discriminant)) / (2 * a), 0.0);

    double t = std::min(t1, t2);

    if (t > 0) {
        Vector3 hit_point = ray.Evaluate(t);
        Vector3 normal = (hit_point - position_).Normalize();

        Intersection intersection = {t, material_, normal, hit_point};
        out_intersection = intersection;
        return true;
    }

    return false;
}
