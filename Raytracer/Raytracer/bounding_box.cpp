#include "pch.h"
#include "bounding_box.h"
#include <algorithm>
#include <cmath>

BoundingBox::BoundingBox() : BoundingBox(INFINITY, -INFINITY, INFINITY, -INFINITY, INFINITY, -INFINITY) {}

BoundingBox::BoundingBox(double singleValue) : BoundingBox(singleValue, singleValue, singleValue, singleValue, singleValue, singleValue) {}

BoundingBox::BoundingBox(Vector3 position) : BoundingBox(position, position) {}

BoundingBox::BoundingBox(Vector3 min, Vector3 max) : BoundingBox(min.GetX(), max.GetX(), min.GetY(), max.GetY(), min.GetZ(), max.GetZ()) {}

BoundingBox::BoundingBox(std::vector<Vector3> points) : BoundingBox() {
    std::vector<BoundingBox> bounding_boxes;
    for (Vector3 point : points) {
        bounding_boxes.push_back(BoundingBox(point));
    }

    ExpandToBound(bounding_boxes);
}

BoundingBox::BoundingBox(double minX, double maxX, double minY, double maxY, double minZ, double maxZ)
    : minX_(minX), maxX_(maxX), minY_(minY), maxY_(maxY), minZ_(minZ), maxZ_(maxZ) {}

BoundingBox::~BoundingBox() = default;

void BoundingBox::ExpandToBound(const BoundingBox& other) {
    minX_ = std::min(minX_, other.minX_);
    minY_ = std::min(minY_, other.minY_);
    minZ_ = std::min(minZ_, other.minZ_);
    maxX_ = std::max(maxX_, other.maxX_);
    maxY_ = std::max(maxY_, other.maxY_);
    maxZ_ = std::max(maxZ_, other.maxZ_);
}

void BoundingBox::ExpandToBound(const std::vector<BoundingBox> bounding_boxes) {
    for (BoundingBox bounding_box : bounding_boxes) {
        ExpandToBound(bounding_box);
    }
}
