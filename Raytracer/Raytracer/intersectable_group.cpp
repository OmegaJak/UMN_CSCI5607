#include "pch.h"
#include "intersectable_group.h"
#include <algorithm>

IntersectableGroup::IntersectableGroup() = default;

IntersectableGroup::~IntersectableGroup() {
    for (Intersectable* child : children_) {
        delete child;
    }
    children_.clear();
}

bool IntersectableGroup::IntersectionWith(const Ray* ray, Intersection* out_intersection) {
    bool found_intersection = false;
    for (Intersectable* child : children_) {
        if (child->IntersectionWith(ray, out_intersection)) {
            found_intersection = true;
        }
    }

    return found_intersection;
}

bool IntersectableGroup::DoesIntersectWith(const Ray& ray) {
    static Intersection dummy_intersection;  // This is used for t logic in case out_intersection is null. Might be a better way to do this
    for (Intersectable* child : children_) {
        dummy_intersection.ray_ = &ray;
        dummy_intersection.ResetT();
        if (child->IntersectionWith(&ray, &dummy_intersection)) {
            return true;
        }
    }

    return false;
}

void IntersectableGroup::AddChild(Intersectable* child) {
    children_.push_back(child);
}

void IntersectableGroup::GenerateBoundingBox() {
    bounding_box_ = BoundingBox();
    for (Intersectable* child : children_) {
        bounding_box_.ExpandToBound(child->GetBoundingBox());
    }
}
