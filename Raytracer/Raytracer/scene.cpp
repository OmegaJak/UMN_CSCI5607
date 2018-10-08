#include "pch.h"
#include "scene.h"
#include <iostream>

Scene::Scene() {}

Scene::~Scene() {
    for (Primitive *primitive : primitives_) {
        delete primitive;
    }
    primitives_.clear();

    for (Light *light : lights_) {
        delete light;
    }
    lights_.clear();
}

bool Scene::FindIntersection(Ray ray, Intersection &out_intersection) {
    for (Primitive *primitive : primitives_) {
        if (primitive->IntersectionWith(ray, out_intersection)) {
            return true;
        }
    }

    return false;
}

Color Scene::GetColor(Intersection intersection, Vector3 viewing_position) {
    Color ambient_contribution = ambient_light_.GetColor();

    Color diffuse_contribution(0, 0, 0);
    Vector3* normal = &intersection.normal;
    Color* diffusion_constant = &intersection.material.diffuse_color_;
    for (Light* light : lights_) {
        if (PointIsAffectedByLight(intersection.hit_point, light)) {
            Vector3 to_light = light->GetPosition() - intersection.hit_point;
            diffuse_contribution += (*diffusion_constant) * (to_light.Dot(*normal)) * light->GetIlluminanceAt(intersection.hit_point);
        }
    }

    Color total_color = ambient_contribution + diffuse_contribution;

    return total_color.Clamp();
}

void Scene::AddPrimitive(Primitive *primitive) {
    primitives_.push_back(primitive);
}

void Scene::SetAmbientLight(AmbientLight ambient_light) {
#ifdef _DEBUG
    std::cout << "Ambient light set to color " << ambient_light.GetColor() << "." << std::endl;
#endif
    ambient_light_ = ambient_light;
}

void Scene::AddLight(Light* light) {
    lights_.push_back(light);
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

bool Scene::PointIsAffectedByLight(Vector3 point, Light* light) {
    return true;
}
