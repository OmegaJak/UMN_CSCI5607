#include "pch.h"
#include "vector3.h"

Vector3::Vector3() {
    x_ = 0;
    y_ = 0;
    z_ = 0.0;
}

Vector3::~Vector3() {}

double Vector3::GetX() {
    return x_;
}

double Vector3::GetY() {
    return y_;
}

double Vector3::GetZ() {
    return z_;
}

void Vector3::Set(double x, double y, double z) {
    x_ = x;
    y_ = y;
    z_ = z;
}
