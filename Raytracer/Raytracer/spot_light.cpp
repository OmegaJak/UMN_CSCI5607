#include "pch.h"
#include "spot_light.h"

SpotLight::SpotLight(Color color, const Vector3& position, const Vector3& direction, double inner_angle_, double outer_angle_)
    : PointLight(color, position), inner_angle_(inner_angle_), outer_angle_(outer_angle_) {
    direction_ = direction.Normalize();
}

SpotLight::~SpotLight() {}

Color SpotLight::GetIlluminanceAt(const Vector3& position) {
    Vector3 to_position = (position - position_).Normalize();
    double angle = acos(to_position.Dot(direction_));
    if (angle < outer_angle_) {
        Color point_illuminance = PointLight::GetIlluminanceAt(position); // This includes some redundant calculations with above
        if (angle <= inner_angle_) return point_illuminance; // Faster than lerping even though a lerp would do this
        
        double lerp_amount = (outer_angle_ - angle) / (outer_angle_ - inner_angle_);
        return Color::Lerp(Color(0, 0, 0), point_illuminance, lerp_amount);
    }

    return Color(0, 0, 0);
}
