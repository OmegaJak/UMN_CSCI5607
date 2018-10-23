#pragma once
#include "Material.h"
#include "ray.h"
#include "intersection.h"

class Primitive {
   public:
    Primitive();
    Primitive(Material material);
    virtual ~Primitive();
    virtual bool IntersectionWith(const Ray* ray, Intersection* out_intersection) = 0;

    void SetMaterial(const Material& material);
    const Material& GetMaterial() const;

   protected:
    Material material_;
};
