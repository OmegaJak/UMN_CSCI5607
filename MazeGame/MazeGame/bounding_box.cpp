#include <algorithm>
#include <cmath>
#include "bounding_box.h"

using std::max;
using std::min;

BoundingBox::BoundingBox() : BoundingBox(INFINITY, -INFINITY, INFINITY, -INFINITY, INFINITY, -INFINITY) {}

BoundingBox::BoundingBox(double singleValue) : BoundingBox(singleValue, singleValue, singleValue, singleValue, singleValue, singleValue) {}

BoundingBox::BoundingBox(glm::vec4 position) : BoundingBox(position, position) {}

BoundingBox::BoundingBox(glm::vec4 min, glm::vec4 max) : BoundingBox(min.x, max.x, min.y, max.y, min.z, max.z) {}

BoundingBox::BoundingBox(std::vector<glm::vec4> points) : BoundingBox() {
    float min_x = INFINITY, min_y = INFINITY, min_z = INFINITY, max_x = -INFINITY, max_y = -INFINITY, max_z = -INFINITY;
    for (auto point : points) {
        min_x = min(min_x, point.x);
        min_y = min(min_y, point.y);
        min_z = min(min_z, point.z);
        max_x = max(max_x, point.x);
        max_y = max(max_y, point.y);
        max_z = max(max_z, point.z);
    }

    InitToBounds(glm::vec3(min_x, min_y, min_z), glm::vec3(max_x, max_y, max_z));
}

// BoundingBox::BoundingBox(const BoundingBox& other)
//    : BoundingBox(other.minX_, other.maxX_, other.minY_, other.maxY_, other.minZ_, other.maxZ_) {}

BoundingBox::BoundingBox(double min_x, double max_x, double min_y, double max_y, double min_z, double max_z) {
    InitToBounds(glm::vec3(min_x, min_y, min_z), glm::vec3(max_x, max_y, max_z));
}

BoundingBox::~BoundingBox() = default;

void BoundingBox::ExpandToBound(const BoundingBox& other) {
    min_.ResetLocalTransform();
    max_.ResetLocalTransform();

    glm::vec3 new_min = glm::vec3(min(min_.X(), other.min_.X()), min(min_.Y(), other.min_.Y()), min(min_.Z(), other.min_.Z()));
    glm::vec3 new_max = glm::vec3(max(max_.X(), other.max_.X()), max(max_.Y(), other.max_.Y()), max(max_.Z(), other.max_.Z()));

    min_.Translate(new_min);
    max_.Translate(new_max);
}

void BoundingBox::ExpandToBound(const std::vector<BoundingBox> bounding_boxes) {
    for (BoundingBox bounding_box : bounding_boxes) {
        ExpandToBound(bounding_box);
    }
}

bool BoundingBox::ContainsOrIntersects(const BoundingBox& other) const {
    return Overlaps(other.min_.X(), other.max_.X(), min_.X(), max_.X()) && Overlaps(other.min_.Y(), other.max_.Y(), min_.Y(), max_.Y()) &&
           Overlaps(other.min_.Z(), other.max_.Z(), min_.Z(), max_.Z());
}

glm::vec3 BoundingBox::Max() const {
    return glm::vec3(max_.X(), max_.Y(), max_.Z());
}

glm::vec3 BoundingBox::Min() const {
    return glm::vec3(min_.X(), min_.Y(), min_.Z());
}

bool BoundingBox::Overlaps(double otherMin, double otherMax, double min, double max) {
    return (IsBetween(otherMin, min, max) || IsBetween(otherMax, min, max)) &&  // Is between or overlapping with
           !(otherMax == otherMin && otherMin < min ||
             otherMin == max && otherMax > max);  // But is not only overlapping on exactly one side
}

bool BoundingBox::IsBetween(double val, double low, double high) {
    return val >= low && val <= high;
}

void BoundingBox::InitToBounds(glm::vec3 min, glm::vec3 max) {
    min_ = Transformable(min);
    max_ = Transformable(max);

    min_.SetParent(&transform);
    max_.SetParent(&transform);
}
