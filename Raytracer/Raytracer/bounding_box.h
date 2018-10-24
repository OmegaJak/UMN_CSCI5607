#pragma once
#include "vector3.h"
#include <vector>

class BoundingBox {
   public:
    BoundingBox();
    explicit BoundingBox(double singleValue);
    explicit BoundingBox(Vector3 position);
    BoundingBox(Vector3 min, Vector3 max); // Gets minX, minY etc from min, does similarly for max
    explicit BoundingBox(std::vector<Vector3> points); // Grows BB to encompass all points
    BoundingBox(double minX, double maxX, double minY, double maxY, double minZ, double maxZ);
    ~BoundingBox();

    void ExpandToBound(const BoundingBox& other);
    void ExpandToBound(const std::vector<BoundingBox> bounding_boxes);

    double minX_, maxX_, minY_, maxY_, minZ_, maxZ_;
};
