#pragma once
#include "Material.h"
#include "materialed.h"
#include "intersectable.h"

class Primitive : public Materialed, public Intersectable {
   public:
    Primitive();
    Primitive(const Material& material);
    virtual ~Primitive();
};
