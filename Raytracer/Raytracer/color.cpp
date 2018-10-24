#include "pch.h"
#include "color.h"
#include <algorithm>

Color::Color() {}

Color::Color(double r, double g, double b) : red_(r), green_(g), blue_(b) {}

Color Color::operator*(const Color& other) const {
    return Color(red_ * other.red_, green_ * other.green_, blue_ * other.blue_);
}

Color Color::operator*(const double& f) const {
    return Color(red_ * f, green_ * f, blue_ * f);
}

Color Color::operator/(const double& f) const {
    return Color(red_ / f, green_ / f, blue_ / f);
}

Color Color::operator+(const Color& other) const {
    return Color(red_ + other.red_, green_ + other.green_, blue_ + other.blue_);
}

Color Color::operator-(const Color& other) const {
    return Color(red_ - other.red_, green_ - other.green_, blue_ - other.blue_);
}

Color& Color::operator+=(const Color& other) {
    red_ += other.red_;
    green_ += other.green_;
    blue_ += other.blue_;

    return *this;
}

Color Color::Lerp(const Color& a, const Color& b, double amount) {
    return a.Lerp(b, amount);
}

Color Color::Lerp(const Color& other, double amount) const {
    return *this + (other - *this) * amount;
}

Color& Color::Clamp() {
    red_ = ComponentClamp(red_);
    green_ = ComponentClamp(green_);
    blue_ = ComponentClamp(blue_);

    return *this;
}

bool Color::IsNotBlack() const {
    return red_ > 0 || green_ > 0 || blue_ > 0;
}

double Color::ComponentClamp(double value) const {
    return std::max(std::min(value, 1.0), 0.0);
}

Color operator*(const double& f, const Color& color) {
    return color * f;
}

std::ostream& operator<<(std::ostream& os, const Color& color) {
    os << "(" << color.red_ << ", " << color.green_ << ", " << color.blue_ << ")";
    return os;
}
