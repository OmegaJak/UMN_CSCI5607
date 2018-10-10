#include "pch.h"
#include "vector3.h"
#include <cmath>
#include <ostream>

Vector3::Vector3() : Vector3(0, 0, 0) {}

Vector3::Vector3(double x, double y, double z) : x_(x), y_(y), z_(z) {}

Vector3::~Vector3() = default;

double Vector3::GetX() const {
    return x_;
}

double Vector3::GetY() const {
    return y_;
}

double Vector3::GetZ() const {
    return z_;
}

void Vector3::Set(double x, double y, double z) {
    x_ = x;
    y_ = y;
    z_ = z;
}

Vector3 Vector3::operator+(const Vector3& other) const {
    return Vector3(x_ + other.GetX(), y_ + other.GetY(), z_ + other.GetZ());
}

Vector3 Vector3::operator-(const Vector3& other) const {
    return Vector3(x_ - other.x_, y_ - other.y_, z_ - other.z_);
}

Vector3 Vector3::operator*(const double& f) const {
    return Vector3(x_ * f, y_ * f, z_ * f);
}

Vector3 operator*(const double& f, const Vector3& vec) {
    return vec * f;
}

std::ostream& operator<<(std::ostream& os, const Vector3& vec) {
    os << "(" << vec.x_ << ", " << vec.y_ << ", " << vec.z_ << ")";
    return os;
}

double Vector3::Dot(const Vector3& other) const {
    return (x_ * other.x_) + (y_ * other.y_) + (z_ * other.z_);
}

Vector3 Vector3::Cross(const Vector3& other) const {
    double x = y_ * other.z_ - z_ * other.y_;
    double y = z_ * other.x_ - x_ * other.z_;
    double z = x_ * other.y_ - y_ * other.x_;
    return Vector3(x, y, z);
}

Vector3 Vector3::Normalize() const {
    double magnitude = Magnitude();
    if (magnitude == 0) {
        printf("Cannot normalize vector of length 0");
        exit(-1);
    }
    return Vector3(x_ / magnitude, y_ / magnitude, z_ / magnitude);
}

double Vector3::Magnitude() const {
    return sqrt(this->Dot(*this));
}

double Vector3::SqrMagnitude() const {
    return pow(Magnitude(), 2);
}
