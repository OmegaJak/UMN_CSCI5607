#pragma once
#include "material.h"

class Materialed {
   public:
    Materialed();
    explicit Materialed(const Material& material);
    virtual ~Materialed();

    void SetMaterial(const Material& material);
    const Material& GetMaterial() const;

   protected:
    Material material_;
};
