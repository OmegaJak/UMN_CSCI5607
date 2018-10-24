#pragma once
#include "vector3.h"
#include <ostream>
#include "directionable.h"

class Ray : public Directionable {
   public:
    Ray();
    Ray(Vector3 start_point, Vector3 direction, double minimum_t = 0.001, double maximum_t = INFINITY);
    ~Ray();

    friend std::ostream& operator<<(std::ostream& os, const Ray& color);

    Vector3 Evaluate(const double& t) const;

    Vector3 start_point_;
    double minimum_t_;
    double maximum_t_;
    Vector3 inverse_dir_; // This is for fast bounding-box intersection
};
