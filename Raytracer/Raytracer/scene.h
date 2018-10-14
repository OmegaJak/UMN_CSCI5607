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
    Color GetColor(const Intersection& intersection, const Vector3& viewing_position) const;

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

    bool IntersectionIsAffectedByLight(const Intersection &intersection, Light* light) const;
};
