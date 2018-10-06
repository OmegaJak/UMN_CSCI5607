#include "pch.h"
#include "scene.h"
#include <iostream>

Scene::Scene() {}

Scene::~Scene() {
    for (Primitive *primitive : primitives_) {
        delete primitive;
    }
    primitives_.clear();
}

bool Scene::FindIntersection(Ray ray, Intersection &out_intersection) {
    for (Primitive *primitive : primitives_) {
        if (primitive->IntersectionWith(ray, out_intersection)) {
            return true;
        }
    }

    return false;
}

Color Scene::GetColor(Material material, Vector3 viewing_position, Vector3 intersection_point) {
    return material.ambient_color_ * ambient_light_;
}

void Scene::AddPrimitive(Primitive *primitive) {
    primitives_.push_back(primitive);
}

void Scene::SetAmbientLight(Color ambient_light) {
#ifdef _DEBUG
    std::cout << "Ambient light set to color " << ambient_light << "." << std::endl;
#endif
    ambient_light_ = ambient_light;
}

void Scene::SetCamera(Camera camera) {
    camera_ = camera;
}

const Camera Scene::GetCamera() {
    return camera_;
}

void Scene::SetCameraAspectRatio(double aspect_ratio) {
    camera_.SetAspectRatio(aspect_ratio);
}
