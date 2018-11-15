#include "transformable.h"

Transformable::Transformable() {
    Reset();
}

Transformable::~Transformable() = default;

void Transformable::Reset() {
    transform_ = glm::mat4();
}

void Transformable::Rotate(float radians, const glm::vec3& around) {
    transform_ = glm::rotate(transform_, radians, around);
}

void Transformable::Translate(glm::vec3 translate_by) {
    transform_ = glm::translate(transform_, translate_by);
}

void Transformable::Scale(const glm::vec3& scale) {
    transform_ = glm::scale(transform_, scale);
}
