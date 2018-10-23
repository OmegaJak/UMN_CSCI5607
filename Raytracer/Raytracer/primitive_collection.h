#pragma once
#include <vector>
#include "primitive.h"
class PrimitiveCollection : public Primitive {
   public:
    PrimitiveCollection();
    ~PrimitiveCollection();

    bool IntersectionWith(const Ray* ray, Intersection* out_intersection) override;
    bool DoesIntersectWith(const Ray& ray);
    void AddPrimitive(Primitive* primitive);

   private:
    std::vector<Primitive*> primitives_{};
};
