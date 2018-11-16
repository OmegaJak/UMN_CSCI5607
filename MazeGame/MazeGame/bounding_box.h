#pragma once
#include <vector>
#include "transformable.h"

class BoundingBox {
   public:
    BoundingBox();
    explicit BoundingBox(double singleValue);
    explicit BoundingBox(glm::vec4 position);
    BoundingBox(glm::vec4 min, glm::vec4 max);            // Gets minX, minY etc from min, does similarly for max
    explicit BoundingBox(std::vector<glm::vec4> points);  // Grows BB to encompass all points
    // BoundingBox(const BoundingBox& other);
    BoundingBox(double minX, double maxX, double minY, double maxY, double minZ, double maxZ);
    ~BoundingBox();

    void ExpandToBound(const BoundingBox& other);
    void ExpandToBound(const std::vector<BoundingBox> bounding_boxes);

    bool ContainsOrIntersects(const BoundingBox& other) const;

    glm::vec3 Max() const;
    glm::vec3 Min() const;

    Transformable transform;

   private:
    static bool Overlaps(double otherMin, double otherMax, double min, double max);
    static bool IsBetween(double val, double low, double high);
    void InitToBounds(glm::vec3 min, glm::vec3 max);

    Transformable min_;
    Transformable max_;
};
