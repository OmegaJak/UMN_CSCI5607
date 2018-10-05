#pragma once
#include "vector3.h"
#include "Material.h"

class Primitive {
   public:
    Primitive();
    Primitive(Vector3 position, Material material);
    ~Primitive();

   protected:
    Vector3 position_;
    Material material_;
};
