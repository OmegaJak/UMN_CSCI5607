#pragma once
#include "primitive.h"
#include "plane.h"

struct BarycentricCoords {
    double alpha, beta, gamma;
};

class Triangle : public Primitive {
   public:
    Triangle(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& a_norm, const Vector3& b_norm, const Vector3& c_norm);
    Triangle(const Vector3& a, const Vector3& b, const Vector3& c);
    ~Triangle();

    bool IntersectionWith(const Ray* ray, Intersection* out_intersection) override;
    void GenerateBoundingBox() override;
   private:
    BarycentricCoords ToBarycentricCoordinates(const Vector3& p) const;

    Vector3 a_, b_, c_;
    Vector3 a_norm_, b_norm_, c_norm_;
    bool has_normalized_verts_ = false;
    Plane plane_;
};
