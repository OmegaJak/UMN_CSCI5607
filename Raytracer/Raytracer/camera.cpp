#include "pch.h"

#include "camera.h"
#include <ctime>

Camera::Camera() : Camera(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0)) {
    srand(time(NULL));
}

Camera::Camera(Vector3 position, Vector3 direction_to_plane, Vector3 absolute_up, double half_frustum_vertical,
    double viewing_plane_distance)
    : Positionable(position), distance_(viewing_plane_distance) {
    forward_ = direction_to_plane.Normalize();
    absolute_up_ = absolute_up.Normalize();

    right_ = forward_.Cross(absolute_up_);
    camera_up_ = right_.Cross(forward_);
    half_frustum_vertical_ = half_frustum_vertical;

    normed_right_ = right_.Normalize();
    normed_up_ = camera_up_.Normalize();

    DoInitialMath();
}

Camera::~Camera() = default;

Ray Camera::ConstructRayThroughPixel(const double& i, const double& j, const double& pixel_width, const double& pixel_height) {
    static Vector3 ray_direction;
    if (IsMoving()) DoInitialMath();

    ray_direction = (GetPixelPoint(i, j, pixel_width, pixel_height) - GetPosition()).Normalize();

    return Ray(GetPosition(), ray_direction);
}

std::vector<Ray> Camera::ConstructRaysThroughPixel(const double& i, const double& j, const double& pixel_width, const double& pixel_height,
    const double& focal_distance, const double& lens_radius, const int& num_samples) {
    static Vector3 u, v, focal_position, lens_position;
    static Ray to_focal_point;
    std::vector<Ray> rays;
    u = normed_right_ * lens_radius;
    v = normed_up_ * lens_radius;

    to_focal_point = ConstructRayThroughPixel(i, j, pixel_width, pixel_height);
    rays.push_back(to_focal_point);
    focal_position = to_focal_point.Evaluate(focal_distance);
    for (int n = 0; n < num_samples - 1; n++) {
        double ru = (rand() / double(RAND_MAX)) * 2 - 1;
        double rv = (rand() / double(RAND_MAX)) * 2 - 1;
        lens_position = ru * u;
        lens_position += rv * v;
        lens_position += GetPosition();
        to_focal_point = Ray(lens_position, (focal_position - lens_position).Normalize());
        rays.push_back(to_focal_point);
    }

    return rays;
}

void Camera::SetAspectRatio(double aspect_ratio) {
    aspect_ratio_ = aspect_ratio;
    DoInitialMath();
}

void Camera::DoInitialMath() {
    Vector3 viewing_plane_center = GetPosition() + distance_ * forward_;
    double vertical_half_height = distance_ * tan(half_frustum_vertical_);
    viewing_plane_top_to_bottom_ = vertical_half_height * camera_up_ * -2;
    viewing_plane_left_to_right_ = vertical_half_height * aspect_ratio_ * right_ * 2;
    top_left_ = viewing_plane_center - (viewing_plane_left_to_right_ * 0.5) - (viewing_plane_top_to_bottom_ * 0.5);
}

const Vector3& Camera::GetPixelPoint(double i, double j, double pixel_width, double pixel_height) {
    static Vector3 horizontal, vertical, pixel_point;
    horizontal = viewing_plane_left_to_right_ * (i / pixel_width);
    vertical = viewing_plane_top_to_bottom_ * (j / pixel_height);
    pixel_point = top_left_ + horizontal + vertical;

    return pixel_point;
}
