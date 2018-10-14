#pragma once

#include <vector>
#include "light.h"
#include "primitive.h"
#include "ray.h"
#include "camera.h"
#include "ambient_light.h"

class Scene {
   public:
    Scene();
    ~Scene();

    bool FindIntersection(const Ray& ray, Intersection& out_intersection) const;
    bool DoesIntersectWith(const Ray& ray) const;
    Color EvaluateRayTree(const Ray& ray, const int& max_recursive_depth) const;
    Color ApplyLightingModel(const Ray& ray, const Intersection& intersection, const int& recursive_depth) const;

    void AddPrimitive(Primitive *primitive);
    void SetAmbientLight(AmbientLight ambient_light);
    void AddLight(Light *light);
    void SetCamera(Camera camera);
    void SetBackgroundColor(Color background);

    const Camera GetCamera();

    void SetCameraAspectRatio(double aspect_ratio);

   private:
    Camera camera_;
    std::vector<Primitive*> primitives_;
    std::vector<Light*> lights_;
    AmbientLight ambient_light_;
    Color background_color_;

    bool IntersectionIsAffectedByLight(const Intersection &intersection, Light* light) const;
};
