#include "pch.h"
#include "scene_object.h"

SceneObject::SceneObject() : SceneObject(Vector3(0, 0, 0)) {}

SceneObject::SceneObject(Vector3 position) : position_(position) {}

void SceneObject::SetPosition(const Vector3 position) {
    position_ = position;
}

const Vector3& SceneObject::GetPosition() {
    return position_;
}

