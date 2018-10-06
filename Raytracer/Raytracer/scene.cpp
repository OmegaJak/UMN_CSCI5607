#include "pch.h"
#include "scene.h"
#include <iostream>


Scene::Scene() {}

Scene::~Scene() = default;

void Scene::AddPrimitive(Primitive primitive) {
    primitives_.push_back(primitive);
}

void Scene::SetAmbientLight(Color ambient_light) {
#ifdef _DEBUG
    std::cout << "Ambient light set of color " << ambient_light << "." << std::endl;
#endif
    ambient_light_ = ambient_light;
}

void Scene::SetBackground(Color background) {
#ifdef _DEBUG
    std::cout << "Background set to color " << background << "." << std::endl;
#endif
    background_ = background;
}
