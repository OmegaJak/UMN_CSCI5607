#include "pch.h"
#include "spot_light.h"

SpotLight::SpotLight(Color color, const Vector3& position, const Vector3& direction, double inner_angle_, double outer_angle_)
    : PointLight(color, position), Directionable(direction), inner_angle_(inner_angle_), outer_angle_(outer_angle_) {
    direction_ = direction.Normalize();
}

SpotLight::~SpotLight() {}

LightRecord SpotLight::GetLightRecordAt(const Vector3& position) {
    auto point_light_record = PointLight::GetLightRecordAt(position);
    double angle = acos(-1 * point_light_record.to_light.Normalize().Dot(direction_)); // -1 because we'd really want to do to_position.Dot(direction), but we already have to_light and this multiplication is faster than vector mult
    if (angle < outer_angle_) {
        if (angle <= inner_angle_) return point_light_record;  // Faster than lerping even though a lerp would do this

        double lerp_amount = (outer_angle_ - angle) / (outer_angle_ - inner_angle_);
        Color color = Color::Lerp(Color(0, 0, 0), point_light_record.illuminance, lerp_amount);
        return LightRecord{point_light_record.to_light, color};
    }

    return LightRecord{point_light_record.to_light, Color(0, 0, 0)};
}
