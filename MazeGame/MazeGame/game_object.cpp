#define GLM_FORCE_RADIANS

#include <algorithm>
#include <functional>
#include <gtc/type_ptr.hpp>
#include "game_object.h"
#include "glad.h"
#include "shader_manager.h"

GameObject::GameObject() {
    model_ = nullptr;
    texture_index_ = UNTEXTURED;
}

GameObject::GameObject(Model* model) : model_(model), texture_index_(UNTEXTURED) {
    material = Material(glm::vec3(1, 0, 1));
    transform = Transformable();

    InitBoundingBox();
}

GameObject::~GameObject() = default;

void GameObject::SetTextureIndex(TEXTURE texture_index) {
    texture_index_ = texture_index;
}

void GameObject::Update() {
    if (model_ == nullptr) {
        printf("GameObject must be given a valid model before calling Update()\n");
        exit(1);
    }

    glUniformMatrix4fv(ShaderManager::Attributes.model, 1, GL_FALSE,
                       glm::value_ptr(transform.WorldTransform()));  // pass model matrix to shader
    glUniform1i(ShaderManager::Attributes.texID, texture_index_);    // Set which texture to use
    if (texture_index_ == UNTEXTURED) {
        glUniform3fv(ShaderManager::Attributes.color, 1, glm::value_ptr(material.color_));  // Update the color, if necessary
    }

    glDrawArrays(GL_TRIANGLES, model_->vbo_vertex_start_index_, model_->NumVerts());
}

void GameObject::InitBoundingBox() {
    std::vector<glm::vec4> vertices = model_->Vertices();  // These are in model space
    std::vector<glm::vec4> world_space_vertices;           // We want them in world space
    world_space_vertices.reserve(vertices.size());         // Preallocate for efficiency
    for (const auto& vertex : vertices) {                  // So convert them one by one
        world_space_vertices.push_back(ToWorldSpace(vertex));
    }

    bounding_box_ = BoundingBox(world_space_vertices);  // Then encompass all verts with a bounding box
}

glm::vec4 GameObject::ToWorldSpace(const glm::vec4& model_coordinate) const {
    return transform.WorldTransform() * model_coordinate;
}
