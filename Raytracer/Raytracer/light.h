#pragma once
#include "color.h"
#include "vector3.h"

struct LightRecord {
    Vector3 to_light; // Not normalized
    Color illuminance;
};

class Light {
   public:
    Light() : Light(Color(1, 0, 1)) {}
    explicit Light(Color color) : color_(color) {}
    virtual ~Light() = default;

    virtual LightRecord GetLightRecordAt(const Vector3& position) = 0;
    const Color& GetColor() const {
        return color_;
    }

   protected:
    Color color_;
};
