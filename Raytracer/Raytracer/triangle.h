#pragma once
#include "primitive.h"

struct BarycentricCoords {
    double alpha, beta, gamma;
};

class Triangle : public Primitive {
   public:
    Triangle(const Vector3& a, const Vector3& b, const Vector3& c, const Material& material, const Vector3& a_norm, const Vector3& b_norm, const Vector3& c_norm);
    Triangle(const Vector3& a, const Vector3& b, const Vector3& c, const Material& material);
    ~Triangle();

    bool IntersectionWith(const Ray* ray, Intersection* out_intersection) override;

   private:
    static bool IntersectionWithPlane(const Ray* ray, const Vector3& normal, const Vector3& point_on_plane, double& out_t);
    BarycentricCoords ToBarycentricCoordinates(const Vector3& p) const;

    Vector3 a_, b_, c_;
    Vector3 normal_;
    Vector3 a_norm_, b_norm_, c_norm_;
    bool has_normalized_verts_ = false;
};
