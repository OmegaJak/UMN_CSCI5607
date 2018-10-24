#pragma once
#include <vector>
#include "vector3.h"
#include "ray.h"

class BoundingBox {
   public:
    BoundingBox();
    explicit BoundingBox(double singleValue);
    explicit BoundingBox(Vector3 position);
    BoundingBox(Vector3 min, Vector3 max);              // Gets minX, minY etc from min, does similarly for max
    explicit BoundingBox(std::vector<Vector3> points);  // Grows BB to encompass all points
    BoundingBox(const BoundingBox& other);
    BoundingBox(double minX, double maxX, double minY, double maxY, double minZ, double maxZ);
    ~BoundingBox();

    void ExpandToBound(const BoundingBox& other);
    void ExpandToBound(const std::vector<BoundingBox> bounding_boxes);

    std::pair<BoundingBox, BoundingBox> Bisect() const;
    std::vector<BoundingBox> Octsect() const;
    bool ContainsOrIntersects(const BoundingBox& other) const;

    bool IntersectsWith(const Ray& ray) const;

    double minX_, maxX_, minY_, maxY_, minZ_, maxZ_;

   private:
    static bool Overlap(double otherMin, double otherMax, double min, double max);
    static bool IsBetween(double val, double low, double high);
};
