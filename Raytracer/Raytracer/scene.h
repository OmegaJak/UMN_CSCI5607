#pragma once

#include <vector>
#include "light.h"
#include "primitive.h"
#include "ray.h"
#include "camera.h"
#include "image.h"

class Scene {
   public:
    Scene();
    ~Scene();

    bool FindIntersection(Ray ray);

    void AddPrimitive(Primitive *primitive);
    void SetAmbientLight(Color ambient_light);
    void SetBackground(Color background);
    void SetCamera(Camera camera);

    const Camera GetCamera();

    void SetCameraAspectRatio(double aspect_ratio);

   private:
    Camera camera_;
    std::vector<Primitive*> primitives_;
    Color ambient_light_;
    Color background_;
};
