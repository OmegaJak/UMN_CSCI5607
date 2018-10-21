#pragma once
#include "ray.h"
#include "vector3.h"
#include "positionable.h"

class Camera : public Positionable {
   public:
    Camera();
    Camera(Vector3 position, Vector3 direction_to_plane, Vector3 absolute_up, double half_frustum_vertical = 45, double viewing_plane_distance = 1);
    ~Camera();

    Ray ConstructRayThroughPixel(const int& i, const int& j, const double& pixel_width, const double& pixel_height) const;
    void SetAspectRatio(double aspect_ratio);

   private:
    Vector3 forward_;
    Vector3 absolute_up_;
    Vector3 camera_up_;
    double half_frustum_vertical_;
    double aspect_ratio_;
    double distance_ = 0.1;

    Vector3 right_;
};
