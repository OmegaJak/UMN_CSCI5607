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

BoundingBox::BoundingBox(const BoundingBox& other)
    : BoundingBox(other.minX_, other.maxX_, other.minY_, other.maxY_, other.minZ_, other.maxZ_) {}

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

std::pair<BoundingBox, BoundingBox> BoundingBox::Bisect() const {
    double x_length = maxX_ - minX_;
    double y_length = maxY_ - minY_;
    double z_length = maxZ_ - minZ_;
    double middle;
    BoundingBox a(*this), b(*this);
    if (x_length > y_length && x_length > z_length) {
        middle = minX_ + x_length / 2.0;
        a.maxX_ = middle;
        b.minX_ = middle;
    } else if (y_length > x_length && y_length > z_length) {
        middle = minY_ + y_length / 2.0;
        a.maxY_ = middle;
        b.minY_ = middle;
    } else {
        middle = minZ_ + z_length / 2.0;
        a.maxZ_ = middle;
        b.minZ_ = middle;
    }

    return std::make_pair(a, b);
}

bool BoundingBox::ContainsOrIntersects(const BoundingBox& other) const {
    return Overlap(other.minX_, other.maxX_, minX_, maxX_) && Overlap(other.minY_, other.maxY_, minY_, maxY_) &&
           Overlap(other.minZ_, other.maxZ_, minZ_, maxZ_);
    //(IsBetween(other.minY_, minY_, maxY_) || IsBetween(other.maxY_, minY_, maxY_)) &&
    //(IsBetween(other.minZ_, minZ_, maxZ_) || IsBetween(other.maxZ_, minZ_, maxZ_));
}

bool BoundingBox::Overlap(double otherMin, double otherMax, double min, double max) {
    return (IsBetween(otherMin, min, max) || IsBetween(otherMax, min, max)) &&  // Is between or overlapping with
           !(otherMax == otherMin && otherMin < min ||
             otherMin == max && otherMax > max);  // But is not only overlapping on exactly one side
}

bool BoundingBox::IsBetween(double val, double low, double high) {
    return val >= low && val <= high;
}
