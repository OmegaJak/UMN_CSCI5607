#include "pch.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include "camera.h"

Camera::Camera() : Camera(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0)) {}

Camera::Camera(Vector3 position, Vector3 direction_to_plane, Vector3 absolute_up, double half_frustum_vertical,
    double viewing_plane_distance)
    : SceneObject(position), distance_(viewing_plane_distance) {
    forward_ = direction_to_plane.Normalize();
    absolute_up_ = absolute_up.Normalize();

    right_ = forward_.Cross(absolute_up_);
    camera_up_ = right_.Cross(forward_);
    half_frustum_vertical_ = (half_frustum_vertical / double(180)) * M_PI;
}

Camera::~Camera() = default;

Ray Camera::ConstructRayThroughPixel(int i, int j, double pixel_width, double pixel_height) const {
    static Vector3 viewing_plane_center = position_ + distance_ * forward_;
    static double vertical_half_height = distance_ * tan(half_frustum_vertical_);
    static Vector3 viewing_plane_top_to_bottom = vertical_half_height * camera_up_ * -2;
    static Vector3 viewing_plane_left_to_right = vertical_half_height * aspect_ratio_ * right_ * -2; // This should not be negative... but has to be to match expected outputs. This suggests something else is wrong...
    static Vector3 top_left = viewing_plane_center - (viewing_plane_left_to_right * 0.5) - (viewing_plane_top_to_bottom * 0.5);
    static Vector3 horizontal, vertical, pixel_point, ray_direction;

    horizontal = viewing_plane_left_to_right * ((i + 0.5) / pixel_width);
    vertical = viewing_plane_top_to_bottom * ((j + 0.5) / pixel_height);
    pixel_point = top_left + horizontal + vertical;
    ray_direction = (pixel_point - position_).Normalize();

    return Ray(position_, ray_direction);
}

void Camera::SetAspectRatio(double aspect_ratio) {
    aspect_ratio_ = aspect_ratio;
}
