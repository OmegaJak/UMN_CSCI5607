#pragma once
#include "vector3.h"

class SceneObject {
   public:
    SceneObject();
    explicit SceneObject(Vector3 position);

    virtual void SetPosition(const Vector3 position);
    virtual const Vector3& GetPosition();

   protected:
    Vector3 position_;
};
