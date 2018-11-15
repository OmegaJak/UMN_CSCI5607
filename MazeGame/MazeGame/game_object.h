#pragma once
#include "material.h"
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

    void Update() override;

    Material material_;

   private:
    Model* model_;
    TEXTURE texture_index_;
};
