#include <gtc/type_ptr.hpp>
#include "game_object.h"
#include "glad.h"
#include "shader_manager.h"

GameObject::GameObject() {}

GameObject::GameObject(Model* model) : model_(model), texture_index_(UNTEXTURED) {
    default_color_ = glm::vec3(1.f, 0.f, 1.f);
}

GameObject::~GameObject() {}

void GameObject::SetTextureIndex(TEXTURE texture_index) {
    texture_index_ = texture_index;
}

void GameObject::SetColor(const glm::vec3& color) {
    default_color_ = color;
}

void GameObject::Update() {
    glUniformMatrix4fv(ShaderManager::Attributes.model, 1, GL_FALSE, glm::value_ptr(transform_));  // pass model matrix to shader
    glUniform1i(ShaderManager::Attributes.texID, texture_index_);                                  // Set which texture to use
    if (texture_index_ == UNTEXTURED) {
        glUniform3fv(ShaderManager::Attributes.color, 1, glm::value_ptr(default_color_));  // Update the color, if necessary
    }

    glDrawArrays(GL_TRIANGLES, model_->vbo_vertex_start_index_, model_->NumVerts());
}
