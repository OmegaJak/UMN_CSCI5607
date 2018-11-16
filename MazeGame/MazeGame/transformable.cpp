#include "transformable.h"

Transformable::Transformable() {
    children_ = std::unordered_set<Transformable*>();
    parent_ = nullptr;
    Reset();
}

Transformable::Transformable(const glm::vec3& position) : Transformable() {
    Translate(position);
}

Transformable::~Transformable() = default;

void Transformable::Reset() {
    ClearChildren();
    ClearParent();
    world_transform_ = local_transform_ = glm::mat4();
}

void Transformable::ResetLocalTransform() {
    local_transform_ = glm::mat4();
    RecalculateWorldTransform();
}

void Transformable::Rotate(float radians, const glm::vec3& around) {
    local_transform_ = glm::rotate(local_transform_, radians, around);
    RecalculateWorldTransform();
}

void Transformable::Translate(const glm::vec3& translate_by) {
    local_transform_ = glm::translate(local_transform_, translate_by);
    RecalculateWorldTransform();
}

void Transformable::Scale(const glm::vec3& scale) {
    local_transform_ = glm::scale(local_transform_, scale);
    RecalculateWorldTransform();
}

void Transformable::AddChild(Transformable* child) {
    children_.insert(child);
    if (!child->IsParent(this)) {
        child->SetParent(this);
    }
}

void Transformable::RemoveChild(Transformable* child) {
    children_.erase(child);
    if (child->IsParent(this)) {
        child->ClearParent();
    }
}

void Transformable::ClearChildren() {
    for (auto child : children_) {
        RemoveChild(child);
    }
}

void Transformable::SetParent(Transformable* parent) {
    if (parent == nullptr) {
        printf("Warning: SetParent was called with a null parent. Should call ClearParent() instead\n");
    }

    parent_ = parent;
    if (!parent_->HasChild(this)) {
        parent_->AddChild(this);
    }
    RecalculateWorldTransform();
}

void Transformable::ClearParent() {
    Transformable* former_parent = parent_;
    parent_ = nullptr;
    if (former_parent != nullptr && former_parent->HasChild(this)) {
        former_parent->RemoveChild(this);
    }
    RecalculateWorldTransform();
}

bool Transformable::HasChild(Transformable* child) const {
    return children_.find(child) != children_.end();
}

bool Transformable::IsParent(Transformable* parent) const {
    return parent_ == parent;
}

void Transformable::RecalculateWorldTransform() {
    if (parent_ == nullptr) {
        world_transform_ = local_transform_;
    } else {
        world_transform_ = parent_->world_transform_ * local_transform_;
        NotifyChildrenOfUpdate();
    }
}

void Transformable::NotifyChildrenOfUpdate() {
    for (Transformable* child : children_) {
        NotifyChildOfUpdate(child);
    }
}

void Transformable::NotifyChildOfUpdate(Transformable* child) {
    if (child == nullptr) {
        printf("A transform had a null child. Exiting...\n");
        exit(1);
    }

    child->RecalculateWorldTransform();
}

glm::mat4 Transformable::LocalTransform() const {
    return local_transform_;
}

glm::mat4 Transformable::WorldTransform() const {
    return world_transform_;
}

float Transformable::X() const {
    return world_transform_[0][3];  // Only translations (stored in the last column of the matrix) will come through if the Transformable is
                                    // interpreted as a position
}

float Transformable::Y() const {
    return world_transform_[1][3];
}

float Transformable::Z() const {
    return world_transform_[2][3];
}
