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
    static Intersection current_intersection;
    static Intersection closest_intersection;
    closest_intersection.t = INFINITY;
    for (Primitive* primitive : primitives_) {
        if (primitive->IntersectionWith(ray, &current_intersection) && current_intersection.t < closest_intersection.t) {
            out_intersection = closest_intersection = current_intersection;
        }
    }

    return closest_intersection.t < INFINITY;
}

bool Scene::DoesIntersectWith(const Ray& ray) const {
    for (Primitive* primitive : primitives_) {
        if (primitive->IntersectionWith(ray, nullptr)) {
            return true;
        }
    }

    return false;
}

Color Scene::GetColor(const Intersection& intersection, const Vector3& viewing_position) const {
    Color diffuse_contribution(0, 0, 0);
    Color specular_contribution(0, 0, 0);
    const Vector3& intersection_point = intersection.hit_point;
    const Vector3& normal = intersection.normal;
    const Material& material = intersection.material;
    Color light_illuminance;

    Color ambient_contribution = ambient_light_.GetColor() * material.ambient_color_;
    for (Light* light : lights_) {
        if (IntersectionIsAffectedByLight(intersection, light)) {
            light_illuminance = light->GetIlluminanceAt(intersection_point);

            Vector3 to_light = (light->GetPosition() - intersection_point).Normalize();
            double light_dot_normal = to_light.Dot(normal);
            diffuse_contribution += material.diffuse_color_ * light_dot_normal * light_illuminance;

            Vector3 perfect_reflection = ((2 * light_dot_normal * normal) - to_light).Normalize();
            Vector3 to_viewer = (viewing_position - intersection_point).Normalize();
            Color toAdd = material.specular_color_ * pow(perfect_reflection.Dot(to_viewer), material.phong_factor_) * light_illuminance;
            specular_contribution += toAdd;
        }
    }

    Color total_color = ambient_contribution + diffuse_contribution.Clamp() + specular_contribution.Clamp();

    return total_color.Clamp();
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

const Camera Scene::GetCamera() {
    return camera_;
}

void Scene::SetCameraAspectRatio(double aspect_ratio) {
    camera_.SetAspectRatio(aspect_ratio);
}

bool Scene::IntersectionIsAffectedByLight(const Intersection& intersection, Light* light) const {
    Ray shadow_ray = Ray(intersection.hit_point + 0.001 * intersection.normal, light->GetPosition() - intersection.hit_point);
    return !DoesIntersectWith(shadow_ray);
}
