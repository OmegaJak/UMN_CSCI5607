#include "pch.h"
#include "color.h"

Color::Color() {}

Color::Color(double r, double g, double b) : red_(r), green_(g), blue_(b) {}

Color::~Color() = default;

std::ostream& operator<<(std::ostream& os, const Color& color) {
    os << "(" << color.red_ << ", " << color.green_ << ", " << color.blue_ << ")";
    return os;
}
