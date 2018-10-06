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

double Sphere::IntersectionWith(const Ray& ray) {
    double a = ray.direction_.SqrMagnitude();
    double b = (2 * ray.direction_).Dot(ray.start_point_ - position_);
    double c = (ray.start_point_ - position_).SqrMagnitude() - (radius_ * radius_);

    double discriminant = (b * b) - 4 * a * c;
    double t1 = (-b + sqrt(discriminant)) / (2 * a);
    double t2 = (-b - sqrt(discriminant)) / (2 * a);

    return std::max(t1, t2);
}
