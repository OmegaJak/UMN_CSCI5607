#pragma once
#include "primitive.h"
class Plane : public Primitive {
   public:
    Plane();
    Plane(const Vector3& normal, const Vector3& point, bool two_sided = true);
    ~Plane();

    bool IntersectionWith(const Ray* ray, Intersection* out_intersection) override;
    int GetNormalFactorToFaceRay(const Ray& ray) const;

    static bool IntersectionWithPlane(const Ray* ray, const Vector3& normal, const Vector3& point_on_plane, double& out_t);

    const Vector3& GetNormal() const {
        return normal_;
    }

   protected:
    Vector3 normal_, point_;
    bool two_sided_;
};
