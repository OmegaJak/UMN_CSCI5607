#include "pch.h"
#include "triangle.h"

Triangle::Triangle(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& a_norm, const Vector3& b_norm,
                                       const Vector3& c_norm)
    : Triangle(a, b, c) {
    a_norm_ = a_norm.Normalize();
    b_norm_ = b_norm.Normalize();
    c_norm_ = c_norm.Normalize();
    has_normalized_verts_ = true;
}

Triangle::Triangle(const Vector3& a, const Vector3& b, const Vector3& c) : a_(a), b_(b), c_(c) {
    Vector3 normal = (b - a).Cross(c - a).Normalize();
    plane_ = Plane(normal, a);
}

Triangle::~Triangle() = default;

bool Triangle::IntersectionWith(const Ray* ray, Intersection* out_intersection) {
    static Intersection plane_intersection; // Can't work in out_intersection in case the point isn't in the triangle
    static Vector3 weighted_normal;
    plane_intersection.ResetT();
    if (plane_.IntersectionWith(ray, &plane_intersection)) {
        BarycentricCoords bary = ToBarycentricCoordinates(plane_intersection.hit_point_);
        bool inside_triangle = (bary.alpha >= 0 && bary.alpha <= 1 && bary.beta >= 0 && bary.gamma <= 1 && bary.alpha + bary.beta <= 1);

        if (inside_triangle && out_intersection->ConsiderIntersection(plane_intersection)) {
            if (has_normalized_verts_) {
                weighted_normal = (bary.gamma * a_norm_);
                weighted_normal += (bary.alpha * b_norm_);
                weighted_normal += (bary.beta * c_norm_).Normalize();
                weighted_normal *= plane_.GetNormalFactorToFaceRay(*ray);
                out_intersection->normal_ = weighted_normal;
            }
            out_intersection->object_ = this;

            return true;
        }
    }

    return false;
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
