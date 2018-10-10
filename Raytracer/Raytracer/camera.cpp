#include "pch.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include "camera.h"


Camera::Camera() : Camera(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0)) {}

Camera::Camera(Vector3 position, Vector3 direction, Vector3 up, double half_frustum_vertical,
    double viewing_plane_distance)
    : SceneObject(position), distance_(viewing_plane_distance) {
    direction_ = direction.Normalize();
    up_ = up.Normalize();
    right_ = direction_.Cross(up_);
    half_frustum_vertical_ = (half_frustum_vertical / double(180)) * M_PI;
}

Camera::~Camera() = default;

Ray Camera::ConstructRayThroughPixel(int i, int j, int pixel_width, int pixel_height) const {
    Vector3 viewing_plane_center = position_ + distance_ * direction_;
    Vector3 viewing_plane_down = distance_ * tan(half_frustum_vertical_) * up_ * -2;
    Vector3 viewing_plane_right = distance_ * tan(half_frustum_vertical_) * aspect_ratio_ * right_ * 2;
    Vector3 top_left = viewing_plane_center - (viewing_plane_right * 0.5) - (viewing_plane_down * 0.5);

    Vector3 horizontal = viewing_plane_right * ((i + 0.5) / double(pixel_width));
    Vector3 vertical = viewing_plane_down * ((j + 0.5) / double(pixel_height));
    Vector3 pixel_point = top_left + horizontal + vertical;
    Vector3 ray_direction = (pixel_point - position_).Normalize();

    return Ray(position_, ray_direction);
}

void Camera::SetAspectRatio(double aspect_ratio) {
    aspect_ratio_ = aspect_ratio;
}
