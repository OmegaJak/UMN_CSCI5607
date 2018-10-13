#include "pch.h"
#include "ray.h"

Ray::Ray() {}

Ray::Ray(Vector3 start_point, Vector3 direction) : start_point_(start_point), direction_(direction) {}

Ray::~Ray() = default;

Vector3 Ray::Evaluate(double t) const {
    return start_point_ + t * direction_;
}

std::ostream& operator<<(std::ostream& os, const Ray& ray) {
    os << ray.start_point_ << " + t * " << ray.direction_;
    return os;
}