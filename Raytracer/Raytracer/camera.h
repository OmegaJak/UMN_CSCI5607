#pragma once
#include "vector3.h"
#include "ray.h"

class Camera {
   public:
    Camera();
    Camera(Vector3 position, Vector3 direction, Vector3 up, double half_frustum, double viewing_plane_distance);
    Camera(Vector3 position, Vector3 direction, Vector3 up, double half_frustum_horizontal, double half_frustum_vertical,
           double viewing_plane_distance);
    ~Camera();

    Ray ConstructRayThroughPixel(int i, int j, int pixel_width, int pixel_height) const;

   private:
    Vector3 position_;
    Vector3 direction_;
    Vector3 up_;
    double half_frustum_vertical_;
    double half_frustum_horizontal_;
    double distance_ = 0.1;

    Vector3 right_;
};
