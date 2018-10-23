#include "pch.h"
#include "triangle.h"

Triangle::Triangle(const Vector3& a, const Vector3& b, const Vector3& c, const Material& material, const Vector3& a_norm, const Vector3& b_norm,
                                       const Vector3& c_norm)
    : Triangle(a, b, c, material) {
    a_norm_ = a_norm.Normalize();
    b_norm_ = b_norm.Normalize();
    c_norm_ = c_norm.Normalize();
    has_normalized_verts_ = true;
}

Triangle::Triangle(const Vector3& a, const Vector3& b, const Vector3& c, const Material& material) : Primitive(material), a_(a), b_(b), c_(c) {
    normal_ = (b - a).Cross(c - a).Normalize();
}

Triangle::~Triangle() = default;

bool Triangle::IntersectionWith(const Ray* ray, Intersection* out_intersection) {
    int normal_factor = 1;
    if (ray->GetDirection().Dot(normal_) > 0) normal_factor = -1; // Used to ensure triangle two-sidedness
    double intersection_t;
    if (IntersectionWithPlane(ray, normal_factor * normal_, a_, intersection_t)) {
        Vector3 hit_point = ray->Evaluate(intersection_t);
        BarycentricCoords bary = ToBarycentricCoordinates(hit_point);
        bool inside_triangle = (bary.alpha >= 0 && bary.alpha <= 1 && bary.beta >= 0 && bary.gamma <= 1 && bary.alpha + bary.beta <= 1);

        if (inside_triangle) {
            out_intersection->ray_ = ray;
            bool valid_intersection = out_intersection->ConsiderT(intersection_t);
            if (!valid_intersection) return false;

            out_intersection->hit_point_ = hit_point;
            if (!has_normalized_verts_) {
                out_intersection->normal_ = normal_ * normal_factor;
            } else {
                Vector3 weighted_normal = ((bary.alpha * a_norm_) + (bary.beta * b_norm_) + (bary.gamma * c_norm_)).Normalize() * normal_factor;
                out_intersection->normal_ = weighted_normal;
            }
            out_intersection->object_ = this;
            out_intersection->ray_ = ray;

            return true;
        }
    }

    return false;
}

// Implementation informed by
// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
bool Triangle::IntersectionWithPlane(const Ray* ray, const Vector3& normal, const Vector3& point_on_plane, double& out_t) {
    double denominator = normal.Dot(ray->GetDirection());
    if (abs(denominator) < 0.000001) return false;

    out_t = (point_on_plane - ray->start_point_).Dot(normal) / denominator;
    return out_t >= 0;
}

// This method came from here:
// https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
BarycentricCoords Triangle::ToBarycentricCoordinates(const Vector3& p) const {
    Vector3 v0 = b_ - a_, v1 = c_ - a_, v2 = p - a_;
    double d00 = v0.Dot(v0);
    double d01 = v0.Dot(v1);
    double d11 = v1.Dot(v1);
    double d20 = v2.Dot(v0);
    double d21 = v2.Dot(v1);
    double denominator = (d00 * d11) - (d01 * d01);
    double alpha = ((d11 * d20) - (d01 * d21)) / denominator;
    double beta = ((d00 * d21) - (d01 * d20)) / denominator;
    double gamma = 1 - alpha - beta;
    return BarycentricCoords{alpha, beta, gamma};
}
