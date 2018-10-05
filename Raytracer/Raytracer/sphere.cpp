#include "pch.h"
#include "sphere.h"
#include <cstdio>

Sphere::Sphere() {}

Sphere::Sphere(Vector3 position, Material material, double radius) : Primitive(position, material)
{
#ifdef _DEBUG
    printf("Sphere created at (%f, %f, %f) with radius %f.\n", position.GetX(), position.GetY(), position.GetZ(), radius);
#endif
}

Sphere::~Sphere() {}
