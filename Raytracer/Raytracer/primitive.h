#pragma once
#include "Material.h"
#include "ray.h"
#include "vector3.h"
#include "scene_object.h"
#include "intersection.h"

class Primitive : public SceneObject {
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
