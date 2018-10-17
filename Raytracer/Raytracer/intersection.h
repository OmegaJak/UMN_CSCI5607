#pragma once
#include "ray.h"

class Primitive;

class Intersection {
   public:
    Intersection();
    Intersection(Ray* ray, Primitive* object, double minimum_t = 0.001);
    ~Intersection();

    bool ConsiderT(const double& t);
    void ResetT();
    double GetT() const;
    Vector3 GetViewingPosition() const;

    const Ray* ray_;
    Primitive* object_;
    Vector3 hit_point_;
    Vector3 normal_;

   private:
    double t_;
    double minimum_t_;
};
