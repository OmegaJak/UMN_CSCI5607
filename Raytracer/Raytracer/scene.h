#pragma once

#include <vector>
#include "light.h"
#include "primitive.h"
#include "sphere.h"
#include "ray.h"

class Scene {
   public:
    Scene();
    ~Scene();

    bool FindIntersection(Ray ray);

    void AddPrimitive(Primitive *primitive);
    void SetAmbientLight(Color ambient_light);
    void SetBackground(Color background);

   private:
    std::vector<Primitive*> primitives_;
    Color ambient_light_;
    Color background_;
};
