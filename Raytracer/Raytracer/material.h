#pragma once
#include "color.h"

class Material {
   public:
    Material();
    Material(Color ambient, Color diffuse, Color specular, Color transmissive, int phong_factor, double index_of_refraction);
    ~Material();

   private:
    Color ambient_color_;
    Color diffuse_color_;
    Color specular_color_;
    Color transmissive_color_;
    int phong_factor_;
    double index_of_refaction_;
};
