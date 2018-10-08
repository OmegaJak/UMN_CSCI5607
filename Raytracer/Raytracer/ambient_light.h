#pragma once
#include "pch.h"
#include "light.h"

class AmbientLight {
   public:
    AmbientLight();
    ~AmbientLight();
    explicit AmbientLight(Color color);

    Color GetColor();

   private:
    Color color_;
};
