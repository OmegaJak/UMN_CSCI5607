#include "pch.h"
#include "primitive_collection.h"

PrimitiveCollection::PrimitiveCollection() = default;

PrimitiveCollection::~PrimitiveCollection() {
    for (Primitive* primitive : primitives_) {
        delete primitive;
    }
    primitives_.clear();
}

bool PrimitiveCollection::IntersectionWith(const Ray* ray, Intersection* out_intersection) {
    bool found_intersection = false;
    for (Primitive* primitive : primitives_) {
        if (primitive->IntersectionWith(ray, out_intersection)) {
            found_intersection = true;
        }
    }

    return found_intersection;
}

bool PrimitiveCollection::DoesIntersectWith(const Ray& ray) {
    static Intersection dummy_intersection;  // This is used for t logic in case out_intersection is null. Might be a better way to do this
    for (Primitive* primitive : primitives_) {
        dummy_intersection.ray_ = &ray;
        dummy_intersection.ResetT();
        if (primitive->IntersectionWith(&ray, &dummy_intersection)) {
            return true;
        }
    }

    return false;
}

void PrimitiveCollection::AddPrimitive(Primitive* primitive) {
    primitives_.push_back(primitive);
}
