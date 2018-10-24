#include "pch.h"
#include "intersectable_group.h"
#include <algorithm>
#include "Rectangle.h"
#include "plane.h"
#include "triangle.h"

IntersectableGroup::IntersectableGroup() = default;

IntersectableGroup::IntersectableGroup(const BoundingBox& initial_bound) {
    bounding_box_ = initial_bound;
    is_bounding_box_initialized_ = true;
}

IntersectableGroup::~IntersectableGroup() = default;  // MEMORY LEAK happens here, Intersectable pointers are never deallocated

bool IntersectableGroup::IntersectionWith(const Ray* ray, Intersection* out_intersection) {
    bool found_intersection = false;
    for (Intersectable* child : children_) {
        if (child->IsInfinite()) {  // Special case for planes, which won't have a bounding box
            if (child->IntersectionWith(ray, out_intersection)) {
                found_intersection = true;
            }
        } else {
            // Skip bounding box checking for leaf nodes, but do it for groups (internal nodes)
            if ((!child->ShouldCheckBoundingBoxBeforeIntersecting() || child->GetBoundingBox().IntersectsWith(*ray)) &&
                child->IntersectionWith(ray, out_intersection)) {
                // Otherwise, first check bounding box, and if we intersect with that, then attempt intersection with the child itself
                found_intersection = true;
            }
        }
    }

    return found_intersection;
}

bool IntersectableGroup::DoesIntersectWith(const Ray& ray) {
    static Intersection dummy_intersection;  // This is used for t logic in case out_intersection is null. Might be a better way to do this
                                             /*dummy_intersection.ray_ = &ray;
                                             dummy_intersection.ResetT();*/

    for (Intersectable* child : children_) {
        dummy_intersection.ray_ = &ray;
        dummy_intersection.ResetT();
        if (child->IsInfinite()) {  // Special case for planes, which won't have a bounding box
            if (child->IntersectionWith(&ray, &dummy_intersection)) {
                return true;
            }
        } else {
            // Skip bounding box checking for leaf nodes, but do it for groups (internal nodes)
            if ((!child->ShouldCheckBoundingBoxBeforeIntersecting() || child->GetBoundingBox().IntersectsWith(ray)) &&
                child->IntersectionWith(&ray, &dummy_intersection)) {
                // Otherwise, first check bounding box, and if we intersect with that, then attempt intersection with the child itself
                return true;
            }
        }
    }
    return false;
}

void IntersectableGroup::AddChild(Intersectable* child) {
    children_.push_back(child);
}

void IntersectableGroup::InitializeBVH() {
    if (children_.size() >= 2) {
        std::vector<Intersectable*> outside_BVH;
        // auto new_boxes = GetBoundingBox().Bisect();  // Bisect our bounding box into two of half size
        // auto* a = new IntersectableGroup(new_boxes.first);
        // auto* b = new IntersectableGroup(new_boxes.second);
        const int child_count = 8;
        auto new_boxes = GetBoundingBox().Octsect();
        std::vector<IntersectableGroup*> new_children;
        for (int i = 0; i < child_count; i++) {
            new_children.push_back(new IntersectableGroup(new_boxes[i]));
        }
        for (Intersectable* child : children_) {
            if (child->IsInfinite()) {  // Ensure infinite objects stay out of the BVH
                outside_BVH.push_back(child);
                continue;
            }

            /*if (new_boxes.first.ContainsOrIntersects(child->GetBoundingBox())) {
                a->AddChild(child);
            }

            if (new_boxes.second.ContainsOrIntersects(child->GetBoundingBox())) {
                b->AddChild(child);
            }*/
            for (int i = 0; i < child_count; i++) {
                if (new_boxes[i].ContainsOrIntersects(child->GetBoundingBox())) {
                    new_children[i]->AddChild(child);
                }
            }
        }

        children_.clear();  // Should NOT delete children from heap, just removes our reference to them
        // if (a->children_.size() > 0) children_.push_back(a);
        // if (b->children_.size() > 0) children_.push_back(b);
        for (int i = 0; i < child_count; i++) {
            if (new_children[i]->children_.size() > 1) {
                children_.push_back(new_children[i]);
                new_children[i]->InitializeBVH();
            } else if (new_children[i]->children_.size() == 1) {
                bool already_in_children = false;
                for (Intersectable* child : children_) {
                    if (new_children[i]->children_[0] == child) already_in_children = true;
                }

                if (!already_in_children) {
                    children_.push_back(new_children[i]->children_[0]);
                }
                delete new_children[i];
            } else {
                delete new_children[i];
            }
        }
        children_.insert(children_.end(), outside_BVH.begin(), outside_BVH.end());  // Re-add the planes

        // Recurse!
        // a->InitializeBVH();
        // b->InitializeBVH();
    }

    // Regenerate our bounding box in case our children's boxes shrunk in the process of setting up the BVH
    GenerateBoundingBox();
}

void IntersectableGroup::GenerateBoundingBox() {
    bounding_box_ = BoundingBox();
    for (Intersectable* child : children_) {
        if (child->IsInfinite()) continue;  // Skip infinite planes, but not inherited types
        bounding_box_.ExpandToBound(child->GetBoundingBox());
    }
}

bool IntersectableGroup::ShouldCheckBoundingBoxBeforeIntersecting() {
    return true;
}
