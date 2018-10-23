#include "pch.h"
#include "intersection.h"
#include <cassert>

Intersection::Intersection() : Intersection(nullptr, nullptr) {}

Intersection::Intersection(Ray* ray, Primitive* object) {
    ray_ = ray;
    object_ = object;
    t_ = INFINITY;
}

Intersection::~Intersection() = default;

bool Intersection::ConsiderT(const double& t) {
    if (ray_ == nullptr || (t > ray_->minimum_t_ && t < ray_->maximum_t_ && t < t_)) {
        t_ = t;
        return true;
    }

    return false;
}

void Intersection::ResetT() {
    t_ = INFINITY;
}

double Intersection::GetT() const {
    return t_;
}

Vector3 Intersection::GetViewingPosition() const {
    return ray_->start_point_;
}

bool Intersection::ConsiderIntersection(const Intersection& other) {
    bool betterT = ConsiderT(other.t_);
    if (!betterT) return false;

    ray_ = other.ray_;
    hit_point_ = other.hit_point_;
    object_ = other.object_;
    normal_ = other.normal_;
    return true;
}
