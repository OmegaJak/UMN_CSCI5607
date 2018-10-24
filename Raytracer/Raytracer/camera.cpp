#include "pch.h"

#include "camera.h"

Camera::Camera() : Camera(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0)) {}

Camera::Camera(Vector3 position, Vector3 direction_to_plane, Vector3 absolute_up, double half_frustum_vertical,
    double viewing_plane_distance)
    : Positionable(position), distance_(viewing_plane_distance) {
    forward_ = direction_to_plane.Normalize();
    absolute_up_ = absolute_up.Normalize();

    right_ = forward_.Cross(absolute_up_);
    camera_up_ = right_.Cross(forward_);
    half_frustum_vertical_ = half_frustum_vertical;

    DoInitialMath();
}

Camera::~Camera() = default;

Ray Camera::ConstructRayThroughPixel(const double& i, const double& j, const double& pixel_width, const double& pixel_height) const {
    static Vector3 horizontal, vertical, pixel_point, ray_direction;

    horizontal = viewing_plane_left_to_right_ * (i / pixel_width);
    vertical = viewing_plane_top_to_bottom_ * (j / pixel_height);
    pixel_point = top_left_ + horizontal + vertical;
    ray_direction = (pixel_point - position_).Normalize();

    return Ray(position_, ray_direction);
}

void Camera::SetAspectRatio(double aspect_ratio) {
    aspect_ratio_ = aspect_ratio;
    DoInitialMath();
}

void Camera::DoInitialMath() {
    Vector3 viewing_plane_center = position_ + distance_ * forward_;
    double vertical_half_height = distance_ * tan(half_frustum_vertical_);
    viewing_plane_top_to_bottom_ = vertical_half_height * camera_up_ * -2;
    viewing_plane_left_to_right_ = vertical_half_height * aspect_ratio_ * right_ * 2;
    top_left_ = viewing_plane_center - (viewing_plane_left_to_right_ * 0.5) - (viewing_plane_top_to_bottom_ * 0.5);
}
