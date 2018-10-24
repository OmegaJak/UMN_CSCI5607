#include "pch.h"
#include "ray.h"

Ray::Ray() {}

Ray::Ray(Vector3 start_point, Vector3 direction, double minimum_t, double maximum_t)
    : Directionable(direction),
      start_point_(start_point),
      minimum_t_(minimum_t), maximum_t_(maximum_t) {
    inverse_dir_.Set(1 / direction_.GetX(), 1 / direction_.GetY(), 1 / direction_.GetZ()); // Precompute
}

Ray::~Ray() = default;

Vector3 Ray::Evaluate(const double& t) const {
    return start_point_ + t * direction_;
}

std::ostream& operator<<(std::ostream& os, const Ray& ray) {
    os << ray.start_point_ << " + t * " << ray.direction_;
    return os;
}
