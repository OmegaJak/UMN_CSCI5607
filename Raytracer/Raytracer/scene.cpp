#include "pch.h"
#include "scene.h"
#include <iostream>

Scene::Scene() {}

Scene::~Scene() {
    for (Primitive* primitive : primitives_) {
        delete primitive;
    }
    primitives_.clear();

    for (Light* light : lights_) {
        delete light;
    }
    lights_.clear();
}

bool Scene::FindIntersection(const Ray& ray, Intersection& out_intersection) const {
    bool found_intersection = false;
    for (Primitive* primitive : primitives_) {
        if (primitive->IntersectionWith(&ray, &out_intersection)) {
            found_intersection = true;
        }
    }

    return found_intersection;
}

bool Scene::DoesIntersectWith(const Ray& ray) const {
    for (Primitive* primitive : primitives_) {
        if (primitive->IntersectionWith(&ray, nullptr)) {
            return true;
        }
    }

    return false;
}

Color Scene::EvaluateRayTree(const Ray& ray, const int& max_recursive_depth) const {
    Intersection intersection;
    if (max_recursive_depth >= 0 && FindIntersection(ray, intersection)) {
        return ApplyLightingModel(ray, intersection, max_recursive_depth);
    }
        
    return background_color_;
}

Color Scene::ApplyLightingModel(const Ray& ray, const Intersection& intersection, const int& recursive_depth) const {
    Color diffuse_contribution(0, 0, 0), specular_contribution(0, 0, 0);
    const Material& material = intersection.object_->GetMaterial();
    Color light_illuminance;

    for (Light* light : lights_) {
        if (IntersectionIsAffectedByLight(intersection, light)) {
            light_illuminance = light->GetIlluminanceAt(intersection.hit_point_);

            Vector3 to_light = (light->GetPosition() - intersection.hit_point_).Normalize();
            double to_light_dot_normal = to_light.Dot(intersection.normal_);
            diffuse_contribution += material.diffuse_color_ * to_light_dot_normal * light_illuminance;

            Vector3 perfect_reflection = ((2 * to_light_dot_normal * intersection.normal_) - to_light).Normalize();
            Vector3 to_viewer = (intersection.GetViewingPosition() - intersection.hit_point_).Normalize();
            specular_contribution +=
                material.specular_color_ * pow(perfect_reflection.Dot(to_viewer), material.phong_factor_) * light_illuminance;
        }
    }

    Ray perfect_reflection = Ray(intersection.hit_point_, ray.direction_.ReflectAbout(intersection.normal_));
    Color reflective_contribution = material.specular_color_ * EvaluateRayTree(perfect_reflection, recursive_depth - 1);



    Color ambient_contribution = ambient_light_.GetColor() * material.ambient_color_;

    return ambient_contribution + diffuse_contribution + specular_contribution + reflective_contribution;
}

void Scene::AddPrimitive(Primitive* primitive) {
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

void Scene::SetBackgroundColor(Color background) {
#ifdef _DEBUG
    std::cout << "Background color set to " << background << "." << std::endl;
#endif
    background_color_ = background;
}

const Camera Scene::GetCamera() {
    return camera_;
}

void Scene::SetCameraAspectRatio(double aspect_ratio) {
    camera_.SetAspectRatio(aspect_ratio);
}

bool Scene::IntersectionIsAffectedByLight(const Intersection& intersection, Light* light) const {
    Ray shadow_ray = Ray(intersection.hit_point_, light->GetPosition() - intersection.hit_point_);
    return !DoesIntersectWith(shadow_ray);
}
