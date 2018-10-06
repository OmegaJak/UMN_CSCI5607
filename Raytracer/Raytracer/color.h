#pragma once
#include <ostream>

class Color {
   public:
    Color();
    Color(double r, double g, double b);
    ~Color();

    Color operator*(const Color& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Color& color);

    double red_, green_, blue_;
};
