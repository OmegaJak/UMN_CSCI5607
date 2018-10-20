#pragma once
#include "color.h"
#include "vector3.h"
#include "scene_object.h"

class Light : public SceneObject {
   public:
    Light() : Light(Color(1, 0, 1), Vector3(0, 0, 100)) {}
    explicit Light(Color color, Vector3 position) : color_(color), SceneObject(position) {}
    virtual ~Light() = default;

    virtual Color GetIlluminanceAt(const Vector3& position) = 0;
    const Color& GetColor() const {
        return color_;
    }

   protected:
    Color color_;
};
