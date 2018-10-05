#pragma once

#include "primitive.h"
#include "sphere.h"
#include "light.h"
#include <vector>

class Scene {
   public:
    Scene();
    ~Scene();

    void AddPrimitive(Primitive primitive);

   private:
    std::vector<Primitive> primitives_;
};
