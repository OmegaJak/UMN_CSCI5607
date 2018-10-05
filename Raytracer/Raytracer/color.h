#pragma once
#include <ostream>

class Color {
   public:
    Color();
    Color(double r, double g, double b);
    ~Color();

    friend std::ostream& operator<<(std::ostream& os, const Color& color);

   private:
    double red_, green_, blue_;
};
