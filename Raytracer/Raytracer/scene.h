#pragma once

#include <vector>
#include "ambient_light.h"
#include "camera.h"
#include "light.h"
#include "primitive.h"
#include "ray.h"
#include "intersectable_group.h"

class Scene {
   public:
    Scene();
    ~Scene();

    bool FindIntersection(const Ray& ray, Intersection& out_intersection);
    bool DoesIntersectWith(const Ray& ray);
    Color EvaluateRayTree(const Ray& ray, const int& max_recursive_depth);
    Color GetRefractiveColor(const Ray& ray, const Intersection& intersection, const Color& reflective_contribution,
                             const int& recursive_depth);
    Color ApplyLightingModel(const Ray& ray, const Intersection& intersection, const int& recursive_depth);

    void AddPrimitive(Primitive* primitive);
    void SetAmbientLight(AmbientLight ambient_light);
    void AddLight(Light* light);
    void SetCamera(Camera camera);
    void SetBackgroundColor(Color background);

    const Camera GetCamera();

    void SetCameraAspectRatio(double aspect_ratio);

   private:
    Camera camera_;
    IntersectableGroup objects_;
    std::vector<Light*> lights_{};
    AmbientLight ambient_light_;
    Color background_color_;

    bool IntersectionIsAffectedByLight(const Intersection& intersection, Light* light);
};
