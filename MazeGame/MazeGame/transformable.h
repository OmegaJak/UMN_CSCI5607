#pragma once

#define GLM_FORCE_RADIANS
#include <unordered_set>
#include "gtc/matrix_transform.hpp"

class Transformable {
   public:
    Transformable();
    Transformable(const glm::vec3& position);
    virtual ~Transformable();

    void Reset();
    void ResetLocalTransform();
    void Rotate(float radians, const glm::vec3& around);
    void Translate(const glm::vec3& translate_by);
    void Scale(const glm::vec3& scale);

    void AddChild(Transformable* child);
    void RemoveChild(Transformable* child);
    void ClearChildren();
    void SetParent(Transformable* parent);
    void ClearParent();
    bool HasChild(Transformable* child) const;
    bool IsParent(Transformable* parent) const;

    void RecalculateWorldTransform();

    void NotifyChildrenOfUpdate();
    static void NotifyChildOfUpdate(Transformable* child);

    glm::mat4 LocalTransform() const;
    glm::mat4 WorldTransform() const;

    float X() const;
    float Y() const;
    float Z() const;

   private:
    glm::mat4 local_transform_;  // This Transformable's local transformation
    glm::mat4 world_transform_;  // This transform in world coordinates, with all parent transformations applied
    Transformable* parent_;
    std::unordered_set<Transformable*> children_;
};
