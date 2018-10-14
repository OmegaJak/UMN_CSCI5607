#include "pch.h"
#include "sphere.h"
#include <algorithm>
#include <cstdio>

Sphere::Sphere() {}

Sphere::Sphere(const Vector3& position, const Material& material, const double radius) : Primitive(position, material) {
    radius_ = radius;
    r_squared_ = radius * radius;
#ifdef _DEBUG
    printf("Sphere created at (%f, %f, %f) with radius %f.\n", position.GetX(), position.GetY(), position.GetZ(), radius);
#endif
}

Sphere::~Sphere() = default;

bool Sphere::IntersectionWith(const Ray& ray, Intersection* out_intersection) {
    double a = ray.direction_.SqrMagnitude();
    Vector3 to_ray = ray.start_point_ - position_;
    double b = (2 * ray.direction_).Dot(to_ray);
    double c = to_ray.SqrMagnitude() - r_squared_;

    double sqrt_discriminant = sqrt((b * b) - 4 * a * c);
    double two_a = 2 * a;
    double t1 = std::max((-b + sqrt_discriminant) / two_a, 0.0);
    double t2 = std::max((-b - sqrt_discriminant) / two_a, 0.0);

    double t = std::min(t1, t2);

    if (t > 0) {
        if (out_intersection != nullptr) {
            Vector3 hit_point = ray.Evaluate(t);

            out_intersection->t = t;
            out_intersection->material = material_;
            out_intersection->normal = (hit_point - position_).Normalize();
            out_intersection->hit_point = hit_point;
            out_intersection->viewing_position = ray.start_point_;
        }
        return true;
    }

    return false;
}
