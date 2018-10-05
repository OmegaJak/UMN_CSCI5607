#include "pch.h"
#include "scene.h"


Scene::Scene() {}

Scene::~Scene() {}

void Scene::AddPrimitive(Primitive primitive) {
    primitives_.push_back(primitive);
}
