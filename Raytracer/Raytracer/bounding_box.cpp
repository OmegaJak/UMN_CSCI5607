#include "pch.h"
#include "bounding_box.h"
#include <algorithm>
#include <cmath>

using std::max;
using std::min;

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

std::vector<BoundingBox> BoundingBox::Octsect() const {
    auto bisected = Bisect();
    auto quadsectedA = bisected.first.Bisect();
    auto quadsectedB = bisected.second.Bisect();

    auto octsectedA = quadsectedA.first.Bisect();
    auto octsectedB = quadsectedA.second.Bisect();
    auto octsectedC = quadsectedB.first.Bisect();
    auto octsectedD = quadsectedB.second.Bisect();

    std::vector<BoundingBox> to_return;
    to_return.push_back(octsectedA.first);
    to_return.push_back(octsectedA.second);
    to_return.push_back(octsectedB.first);
    to_return.push_back(octsectedB.second);
    to_return.push_back(octsectedC.first);
    to_return.push_back(octsectedC.second);
    to_return.push_back(octsectedD.first);
    to_return.push_back(octsectedD.second);

    return to_return;
}

bool BoundingBox::ContainsOrIntersects(const BoundingBox& other) const {
    return Overlap(other.minX_, other.maxX_, minX_, maxX_) && Overlap(other.minY_, other.maxY_, minY_, maxY_) &&
           Overlap(other.minZ_, other.maxZ_, minZ_, maxZ_);
}

// Credit for this method goes to https://gamedev.stackexchange.com/a/18459 and https://tavianator.com/fast-branchless-raybounding-box-intersections/
bool BoundingBox::IntersectsWith(const Ray& ray) const {
    double tx1 = (minX_ - ray.start_point_.GetX()) * ray.inverse_dir_.GetX();
    double tx2 = (maxX_ - ray.start_point_.GetX()) * ray.inverse_dir_.GetX();
    double tmin = min(tx1, tx2);
    double tmax = max(tx1, tx2);

    double ty1 = (minY_ - ray.start_point_.GetY()) * ray.inverse_dir_.GetY();
    double ty2 = (maxY_ - ray.start_point_.GetY()) * ray.inverse_dir_.GetY();
    tmin = max(tmin, min(min(ty1, ty2), tmax));
    tmax = min(tmax, max(max(ty1, ty2), tmin));

    double tz1 = (minZ_ - ray.start_point_.GetZ()) * ray.inverse_dir_.GetZ();
    double tz2 = (maxZ_ - ray.start_point_.GetZ()) * ray.inverse_dir_.GetZ();
    tmin = max(tmin, min(min(tz1, tz2), tmax));
    tmax = min(tmax, max(max(tz1, tz2), tmin));

    //double tmin = max(max(min(tx1, tx2), min(ty1, ty2)), min(tz1, tz2));
    //double tmax = min(min(max(tx1, tx2), max(ty1, ty2)), max(tz1, tz2));

    if (tmax < 0 || tmin > tmax) {
        return false;
    }
    return true;
    //return tmax > max(tmin, 0.0);
}

bool BoundingBox::Overlap(double otherMin, double otherMax, double min, double max) {
    return (IsBetween(otherMin, min, max) || IsBetween(otherMax, min, max)) &&  // Is between or overlapping with
           !(otherMax == otherMin && otherMin < min ||
             otherMin == max && otherMax > max);  // But is not only overlapping on exactly one side
}

bool BoundingBox::IsBetween(double val, double low, double high) {
    return val >= low && val <= high;
}
