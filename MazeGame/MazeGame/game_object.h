#pragma once
#include "model.h"
#include "texture_manager.h"
#include "transformable.h"
#include "updatable.h"

class GameObject : public Transformable, public Updatable {
   public:
    GameObject();
    explicit GameObject(Model* model);
    virtual ~GameObject();

    void SetTextureIndex(TEXTURE texture_index);
    void SetColor(const glm::vec3& color);

    void Update() override;

   private:
    Model* model_;
    TEXTURE texture_index_;
    glm::vec3 default_color_;
};
