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

    bool FindIntersection(Ray ray, Intersection& out_intersection);
    Color GetColor(Material material, Vector3 viewing_position, Vector3 intersection_point);

    void AddPrimitive(Primitive *primitive);
    void SetAmbientLight(Color ambient_light);
    void SetCamera(Camera camera);

    const Camera GetCamera();

    void SetCameraAspectRatio(double aspect_ratio);

   private:
    Camera camera_;
    std::vector<Primitive*> primitives_;
    Color ambient_light_;
};
