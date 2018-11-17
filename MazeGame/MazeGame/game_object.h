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
    bool IntersectsWith(const GameObject& other) const;

    Material material;
    std::shared_ptr<Transformable> transform;

   protected:
    void InitBoundingBox(const std::vector<glm::vec4>& vertices);
    glm::vec4 ToWorldSpace(const glm::vec4& model_coordinate) const;

    std::shared_ptr<BoundingBox> bounding_box_;
    Model* model_;
    TEXTURE texture_index_;
};
