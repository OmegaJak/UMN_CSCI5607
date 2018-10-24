#pragma once
#include <vector>
#include "intersectable.h"

class IntersectableGroup : public Intersectable {
   public:
    IntersectableGroup();
    explicit IntersectableGroup(const BoundingBox& initial_bound);
    ~IntersectableGroup();

    bool IntersectionWith(const Ray* ray, Intersection* out_intersection) override;
    bool DoesIntersectWith(const Ray& ray);
    void AddChild(Intersectable* child);

    void InitializeBVH();
    void GenerateBoundingBox() override;
    bool ShouldCheckBoundingBoxBeforeIntersecting() override;
   private:
    std::vector<Intersectable*> children_{};
};
