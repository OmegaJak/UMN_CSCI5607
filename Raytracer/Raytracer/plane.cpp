#include "pch.h"
#include "plane.h"

Plane::Plane() : Plane(Vector3(0, 0, 1), Vector3()) {}

Plane::Plane(const Vector3& normal, const Vector3& point, bool two_sided) : normal_(normal.Normalize()), point_(point), two_sided_(two_sided) {}

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
        out_intersection->normal_ = two_sided_ ? normal_ * normal_factor : normal_;

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

void Plane::GenerateBoundingBox() {
    bounding_box_ = BoundingBox(0); // Planes are infinite, therefore, bounding them doesn't really make sense. An implementation could be imagined that did bound them, but that would be more difficult.
}
