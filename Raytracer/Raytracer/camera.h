#pragma once
#include "ray.h"
#include "vector3.h"
#include "positionable.h"
#include <vector>

class Camera : public Positionable {
   public:
    Camera();
    Camera(Vector3 position, Vector3 direction_to_plane, Vector3 absolute_up, double half_frustum_vertical = 45, double viewing_plane_distance = 1);
    ~Camera();

    Ray ConstructRayThroughPixel(const double& i, const double& j, const double& pixel_width, const double& pixel_height);
    std::vector<Ray> ConstructRaysThroughPixel(const double& i, const double& j, const double& pixel_width, const double& pixel_height,
                                               const double& focal_distance, const double& lens_radius, const int& num_samples);
    void SetAspectRatio(double aspect_ratio);

   private:
    void DoInitialMath();
    const Vector3& GetPixelPoint(double i, double j, double pixel_width, double pixel_height);

    Vector3 forward_;
    Vector3 absolute_up_;
    Vector3 camera_up_, normed_up_;
    double half_frustum_vertical_;
    double aspect_ratio_;
    double distance_ = 0.1;

    Vector3 right_, normed_right_;

    Vector3 viewing_plane_top_to_bottom_, viewing_plane_left_to_right_, top_left_;
};
