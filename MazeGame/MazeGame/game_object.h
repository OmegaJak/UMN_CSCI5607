#pragma once
#include "bounding_box.h"
#include "material.h"
#include "model.h"
#include "texture_manager.h"
#include "transformable.h"
#include "updatable.h"

class GameObject : public Updatable {
   public:
    GameObject();
    explicit GameObject(Model* model);
    virtual ~GameObject();

    void SetTextureIndex(TEXTURE texture_index);

    void Update() override;

    Material material;
    Transformable transform;

   private:
    void InitBoundingBox();
    glm::vec4 ToWorldSpace(const glm::vec4& model_coordinate) const;

    Model* model_;
    TEXTURE texture_index_;
    BoundingBox bounding_box_;
};
