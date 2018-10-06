#include "pch.h"
#include "scene.h"
#include <iostream>

Scene::Scene() {}

Scene::~Scene() {
    for (Primitive *primitive : primitives_) {
        delete primitive;
    }
    primitives_.clear();
}  // Destroy primitives here??

bool Scene::FindIntersection(Ray ray) {
    for (Primitive *primitive : primitives_) {
        if (primitive->IntersectionWith(ray) >= 0) {
            return true;
        }
    }

    return false;
}

void Scene::AddPrimitive(Primitive *primitive) {
    primitives_.push_back(primitive);
}

void Scene::SetAmbientLight(Color ambient_light) {
#ifdef _DEBUG
    std::cout << "Ambient light set of color " << ambient_light << "." << std::endl;
#endif
    ambient_light_ = ambient_light;
}

void Scene::SetBackground(Color background) {
#ifdef _DEBUG
    std::cout << "Background set to color " << background << "." << std::endl;
#endif
    background_ = background;
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
