#pragma once
#include <ostream>

class Color {
   public:
    Color();
    Color(double r, double g, double b);

    Color operator*(const Color& other) const;
    Color operator*(const double& f) const;
    Color operator+(const Color& other) const;
    Color operator-(const Color& other) const;
    Color& operator+=(const Color& other);
    friend Color operator*(const double& f, const Color& color);
    friend std::ostream& operator<<(std::ostream& os, const Color& color);

    static Color Lerp(const Color& a, const Color& b, double amount);
    Color Lerp(const Color& other, double amount) const;

    Color& Clamp();
    bool IsNotBlack() const;

    double red_, green_, blue_;

   private:
    double ComponentClamp(double value) const;
};
