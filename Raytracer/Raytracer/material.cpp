#include "pch.h"
#include "material.h"
#include <iostream>

Material::Material() {}

Material::Material(Color &ambient, Color &diffuse, Color &specular, Color &transmissive, int phong_factor, double index_of_refraction)
    : ambient_color_(ambient),
      diffuse_color_(diffuse),
      specular_color_(specular),
      transmissive_color_(transmissive),
      phong_factor_(phong_factor),
      index_of_refraction_(index_of_refraction) {
#ifdef _DEBUG
    std::cout << "Material created with ambient " << ambient << ", diffuse " << diffuse << ", specular " << specular
              << ", and transmissive " << transmissive << ",\n\twith a Phong factor of " << phong_factor
              << " and an index of refraction of " << index_of_refraction << "." << std::endl;
#endif
}

Material::~Material() = default;
