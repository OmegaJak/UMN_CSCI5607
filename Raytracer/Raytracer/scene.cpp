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

    Color diffuse_contribution(0, 0, 0);
    Color specular_contribution(0, 0, 0);
    Vector3& intersection_point = intersection.hit_point;
    Vector3& normal = intersection.normal;
    Material& material = intersection.material;
    Color light_illuminance;

    Color ambient_contribution = ambient_light_.GetColor() * material.ambient_color_;
    for (Light* light : lights_) {
        if (PointIsAffectedByLight(intersection_point, light)) {
            light_illuminance = light->GetIlluminanceAt(intersection_point);

            Vector3 to_light = (light->GetPosition() - intersection_point).Normalize();
            double light_dot_normal = to_light.Dot(normal);
            diffuse_contribution += material.diffuse_color_ * light_dot_normal * light_illuminance;

            Vector3 perfect_reflection = ((2 * light_dot_normal * normal) - to_light).Normalize();
            Vector3 to_viewer = (viewing_position - intersection_point).Normalize();
            Color toAdd = material.specular_color_ * pow(perfect_reflection.Dot(to_viewer), material.phong_factor_) * light_illuminance;
            if (toAdd.red_ < 0 || toAdd.blue_ < 0 || toAdd.green_ < 0) {
                //std::cout << toAdd << std::endl;
            }
            specular_contribution += toAdd;
        }
    }

    Color total_color = ambient_contribution + diffuse_contribution.Clamp() + specular_contribution.Clamp();

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
