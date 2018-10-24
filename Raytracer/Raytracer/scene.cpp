#include "pch.h"
#include "scene.h"
#include <algorithm>
#include <iostream>

Scene::Scene() {}

Scene::~Scene() {
    for (Light* light : lights_) {
        delete light;
    }
    lights_.clear();
}

bool Scene::FindIntersection(const Ray& ray, Intersection& out_intersection) {
    return objects_.IntersectionWith(&ray, &out_intersection);
}

bool Scene::DoesIntersectWith(const Ray& ray) {
    return objects_.DoesIntersectWith(ray);
}

Color Scene::EvaluateRayTree(const Ray& ray, const int& max_recursive_depth) {
    Intersection intersection;
    if (max_recursive_depth >= 0 && FindIntersection(ray, intersection)) {
        return ApplyLightingModel(ray, intersection, max_recursive_depth);
    }

    return background_color_;
}

bool Refract(const Vector3& d, const Vector3& n, const double& ior, Ray& refracted_ray, const Vector3& ray_start) {
    Vector3 first_term = ior * (d - n * (d.Dot(n)));
    double under_sqrt = 1 - pow(ior, 2) * (1 - pow(d.Dot(n), 2));
    if (under_sqrt < 0) return false;

    Vector3 refracted_direction = first_term - n * sqrt(under_sqrt);
    refracted_ray = Ray(ray_start, refracted_direction);

    return true;
}

Color Scene::GetRefractiveColor(const Ray& ray, const Intersection& intersection, const Color& reflective_contribution,
                                const int& recursive_depth) {
    Color transmissive_color;
    const Material& material = intersection.object_->GetMaterial();
    // This uses the variable name and methodology found in Fundamental of Computer Graphics, 4th edition
    if (material.transmissive_color_.IsNotBlack()) {
        double d_dot_n = ray.GetDirection().Dot(intersection.normal_);
        double cos;
        Ray refracted_ray;
        if (d_dot_n < 0) {  // Outside object entering
            Refract(ray.GetDirection(), intersection.normal_, 1 / material.index_of_refraction_, refracted_ray, intersection.hit_point_);
            cos = -d_dot_n;
            transmissive_color = Color(1, 1, 1);
        } else {  // Inside object leaving
            double t = intersection.GetT();
            Color A = Color(log(1.1), log(1.05), log(1.1));
            transmissive_color = Color(exp(-t * A.red_), exp(-t * A.green_), exp(-t * A.blue_));
            if (Refract(ray.GetDirection(), -1 * intersection.normal_, material.index_of_refraction_, refracted_ray,
                        intersection.hit_point_)) {
                cos = refracted_ray.GetDirection().Dot(intersection.normal_);
            } else {
                return transmissive_color * EvaluateRayTree(refracted_ray, recursive_depth - 1);
            }
        }

        double R_naught = pow(material.index_of_refraction_ - 1, 2) / pow(material.index_of_refraction_ + 1, 2);
        double R = R_naught + (1 - R_naught) * pow((1 - cos), 5);
        return transmissive_color * (R * reflective_contribution + (1 - R) * EvaluateRayTree(refracted_ray, recursive_depth - 1));
    }

    return Color(0, 0, 0);
}

Color Scene::ApplyLightingModel(const Ray& ray, const Intersection& intersection, const int& recursive_depth) {
    Color diffuse_contribution(0, 0, 0), specular_contribution(0, 0, 0);
    const Material& material = intersection.object_->GetMaterial();

    Ray perfect_reflection = Ray(intersection.hit_point_, ray.GetDirection().ReflectAbout(intersection.normal_).Normalize());
    Color reflective_contribution = material.specular_color_ * EvaluateRayTree(perfect_reflection, recursive_depth - 1);

    Color refractive_contribution =
        material.transmissive_color_ * GetRefractiveColor(ray, intersection, reflective_contribution, recursive_depth);

    for (Light* light : lights_) {
        if (IntersectionIsAffectedByLight(intersection, light)) {
            auto light_record = light->GetLightRecordAt(intersection.hit_point_);

            Vector3 to_light_normalized = light_record.to_light.Normalize();
            double to_light_dot_normal = to_light_normalized.Dot(intersection.normal_);
            diffuse_contribution += material.diffuse_color_ * std::max(0.0, to_light_dot_normal) * light_record.illuminance;

            // Blinn Phong
            /*Vector3 to_viewer = (intersection.GetViewingPosition() - intersection.hit_point_).Normalize();
            Vector3 to_viewer_plus_to_light = to_viewer + to_light_normalized;
            Vector3 h = to_viewer_plus_to_light.Normalize();
            specular_contribution += material.specular_color_ *
                                     pow(std::max(0.0, intersection.normal_.Dot(h)), material.phong_factor_) * light_record.illuminance;*/
            Vector3 light_reflected = (-1 * to_light_normalized).ReflectAbout(intersection.normal_).Normalize();
            Vector3 to_viewer = (intersection.GetViewingPosition() - intersection.hit_point_).Normalize();
            specular_contribution += material.specular_color_ * pow(std::max(0.0, to_viewer.Dot(light_reflected)), material.phong_factor_) *
                                     light_record.illuminance;
        }
    }

    Color ambient_contribution = ambient_light_.GetColor() * material.ambient_color_;

    return (ambient_contribution + diffuse_contribution + specular_contribution + reflective_contribution + refractive_contribution)
        .Clamp();
}

void Scene::AddPrimitive(Primitive* primitive) {
    objects_.AddChild(primitive);
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

bool Scene::IntersectionIsAffectedByLight(const Intersection& intersection, Light* light) {
    Ray shadow_ray = Ray(intersection.hit_point_, light->GetLightRecordAt(intersection.hit_point_).to_light);
    if (dynamic_cast<Positionable*>(
            light))  // If the light has a position, then the direction vector of the ray is not normalized. So t = 1 is at light
        shadow_ray.maximum_t_ = 1;
    else  // If the light does not have a position (probably directional), it's infinitely far away
        shadow_ray.maximum_t_ = INFINITY;
    return !DoesIntersectWith(shadow_ray);
}
