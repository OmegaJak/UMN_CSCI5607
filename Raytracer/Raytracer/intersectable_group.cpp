#include "pch.h"
#include "intersectable_group.h"
#include <algorithm>
#include "plane.h"
#include "Rectangle.h"
#include "triangle.h"

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

void IntersectableGroup::InitializeBVH() {
    std::vector<Intersectable*> outside_BVH;
    auto* a = new IntersectableGroup();
    auto* b = new IntersectableGroup();
    if (children_.size() > 2) {
        auto new_boxes = GetBoundingBox().Bisect(); // Bisect our bounding box into two of half size
        for (Intersectable* child : children_) {
            if (child->IsInfinite()) { // Ensure infinite objects stay out of the BVH
                outside_BVH.push_back(child);
                continue;
            }

            if (new_boxes.first.ContainsOrIntersects(child->GetBoundingBox())) {
                a->AddChild(child);
            }

            if (new_boxes.second.ContainsOrIntersects(child->GetBoundingBox())) {
                b->AddChild(child);
            }
        }

        children_.clear(); // Should NOT delete children from heap, just removes our reference to them
        children_.push_back(a);
        children_.push_back(b);
        children_.insert(children_.end(), outside_BVH.begin(), outside_BVH.end()); // Re-add the planes

        // Recurse!
        a->InitializeBVH();
        b->InitializeBVH();

        // Regenerate our bounding box in case our children's boxes shrunk in the process of setting up the BVH
        GenerateBoundingBox();
    } 
}

void IntersectableGroup::GenerateBoundingBox() {
    bounding_box_ = BoundingBox();
    for (Intersectable* child : children_) {
        if (child->IsInfinite()) continue; // Skip infinite planes, but not inherited types
        bounding_box_.ExpandToBound(child->GetBoundingBox());
    }
}
