#pragma once
#include "ray.h"
#include "vector3.h"

class Camera {
   public:
    Camera();
    Camera(Vector3 position, Vector3 direction, Vector3 up, double half_frustum_vertical = 45, double viewing_plane_distance = 1);
    ~Camera();

    Ray ConstructRayThroughPixel(int i, int j, int pixel_width, int pixel_height) const;
    void SetAspectRatio(double aspect_ratio);

   private:
    Vector3 position_;
    Vector3 direction_;
    Vector3 up_;
    double half_frustum_vertical_;
    double aspect_ratio_;
    double distance_ = 0.1;

    Vector3 right_;
};
