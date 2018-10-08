#pragma once

#include <vector>
#include "light.h"
#include "primitive.h"
#include "ray.h"
#include "camera.h"
#include "image.h"
#include "ambient_light.h"

class Scene {
   public:
    Scene();
    ~Scene();

    bool FindIntersection(Ray ray, Intersection& out_intersection);
    Color GetColor(Intersection intersection, Vector3 viewing_position);

    void AddPrimitive(Primitive *primitive);
    void SetAmbientLight(AmbientLight ambient_light);
    void AddLight(Light *light);
    void SetCamera(Camera camera);

    const Camera GetCamera();

    void SetCameraAspectRatio(double aspect_ratio);

   private:
    Camera camera_;
    std::vector<Primitive*> primitives_;
    std::vector<Light*> lights_;
    AmbientLight ambient_light_;

    bool PointIsAffectedByLight(Vector3 point, Light* light);
};
