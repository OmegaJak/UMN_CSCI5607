#pragma once

#include <vector>
#include "light.h"
#include "primitive.h"
#include "sphere.h"

class Scene {
   public:
    Scene();
    ~Scene();

    void AddPrimitive(Primitive primitive);
    void SetAmbientLight(Color ambient_light);
    void SetBackground(Color background);

   private:
    std::vector<Primitive> primitives_;
    Color ambient_light_;
    Color background_;
};
