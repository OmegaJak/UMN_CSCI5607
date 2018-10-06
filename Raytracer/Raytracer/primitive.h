#pragma once
#include "vector3.h"
#include "Material.h"
#include "ray.h"

class Primitive {
   public:
    Primitive();
    Primitive(Vector3 position, Material material);
    ~Primitive();
    virtual double IntersectionWith(const Ray& ray) = 0;

protected:
    Vector3 position_;
    Material material_;
};
