#pragma once
#include "Material.h"
#include "ray.h"
#include "vector3.h"
#include "scene_object.h"

struct Intersection {
    double t;
    Material material;
    Vector3 normal;
    Vector3 hit_point;
};

class Primitive : public SceneObject {
   public:
    Primitive();
    Primitive(Vector3 position, Material material);
    virtual ~Primitive();
    virtual bool IntersectionWith(const Ray& ray, Intersection* out_intersection) = 0;

   protected:
    Material material_;
};
