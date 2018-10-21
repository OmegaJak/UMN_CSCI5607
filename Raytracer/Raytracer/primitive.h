#pragma once
#include "Material.h"
#include "ray.h"
#include "vector3.h"
#include "positionable.h"
#include "intersection.h"

class Primitive : public Positionable {
   public:
    Primitive();
    Primitive(Vector3 position, Material material);
    virtual ~Primitive();
    virtual bool IntersectionWith(const Ray* ray, Intersection* out_intersection) = 0;

    Material GetMaterial() const {
        return material_;
    }

   protected:
    Material material_;
};
