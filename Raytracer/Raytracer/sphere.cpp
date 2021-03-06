#include "pch.h"
#include "sphere.h"

Sphere::Sphere() {}

Sphere::Sphere(const Vector3& position, const double radius) : Positionable(position) {
    radius_ = radius;
    r_squared_ = radius * radius;
#ifdef _DEBUG
    printf("Sphere created at (%f, %f, %f) with radius %f.\n", position.GetX(), position.GetY(), position.GetZ(), radius);
#endif
}

Sphere::~Sphere() = default;

bool Sphere::IntersectionWith(const Ray* ray, Intersection* out_intersection) {
    double a = ray->GetDirection().SqrMagnitude();
    Vector3 to_ray = ray->start_point_ - GetPosition();
    double b = (2 * ray->GetDirection()).Dot(to_ray);
    double c = to_ray.SqrMagnitude() - r_squared_;

    double sqrt_discriminant = sqrt((b * b) - 4 * a * c);
    double two_a = 2 * a;
    double t1 = (-b + sqrt_discriminant) / two_a;
    double t2 = (-b - sqrt_discriminant) / two_a;

    out_intersection->ray_ = ray;
    bool t1_valid = out_intersection->ConsiderT(t1);
    bool t2_valid = out_intersection->ConsiderT(t2);

    bool did_intersect = t1_valid || t2_valid;
    if (did_intersect && out_intersection != nullptr) {
        Vector3 hit_point = ray->Evaluate(out_intersection->GetT());

        out_intersection->object_ = this;
        out_intersection->hit_point_ = hit_point;
        out_intersection->normal_.Set((hit_point - GetPosition()).Normalize());
    }

    return did_intersect;
}
