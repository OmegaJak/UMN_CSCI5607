#include "pch.h"
#include "plane.h"

Plane::Plane() : Plane(Vector3(0, 0, 1), Vector3()) {}

Plane::Plane(const Vector3& normal, const Vector3& point) : normal_(normal), point_(point) {}

Plane::~Plane() = default;

bool Plane::IntersectionWith(const Ray* ray, Intersection* out_intersection) {
    double intersection_t;
    int normal_factor = GetNormalFactorToFaceRay(*ray);
    if (IntersectionWithPlane(ray, normal_factor * normal_, point_, intersection_t)) {
        out_intersection->ray_ = ray;
        if (!out_intersection->ConsiderT(intersection_t)) return false;
        out_intersection->ray_ = ray;
        out_intersection->hit_point_ = ray->Evaluate(intersection_t);
        out_intersection->object_ = this;
        out_intersection->normal_ = normal_ * normal_factor;

        return true;
    }

    return false;
}

int Plane::GetNormalFactorToFaceRay(const Ray& ray) const {
    if (ray.GetDirection().Dot(normal_) > 0) return -1;
    return 1;
}

// Implementation informed by
// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
bool Plane::IntersectionWithPlane(const Ray* ray, const Vector3& normal, const Vector3& point_on_plane, double& out_t) {
    double denominator = normal.Dot(ray->GetDirection());
    if (abs(denominator) < 0.000001) return false;

    out_t = (point_on_plane - ray->start_point_).Dot(normal) / denominator;
    return out_t >= 0;
}
