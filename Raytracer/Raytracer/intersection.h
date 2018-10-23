#pragma once
#include "ray.h"

class Primitive;

class Intersection {
   public:
    Intersection();
    Intersection(Ray* ray, Primitive* object);
    ~Intersection();

    bool ConsiderT(const double& t);
    void ResetT();
    double GetT() const;
    Vector3 GetViewingPosition() const;

    void Set(const Intersection& other);

    const Ray* ray_;
    Primitive* object_;
    Vector3 hit_point_;
    Vector3 normal_;

   private:
    double t_;
};
