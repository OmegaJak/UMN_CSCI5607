#include <fstream>
#include <iostream>
#include "constants.h"
#include "model.h"
#include "model_manager.h"

Model::Model(const std::string& file) {
    Load(file);
    ModelManager::RegisterModel(this);
}

void Model::Load(const std::string& file) {
    std::ifstream modelFile;
    modelFile.open(file);
    int num_elements;
    modelFile >> num_elements;
    model_ = new float[num_elements];

    for (int i = 0; i < num_elements; i++) {
        modelFile >> model_[i];
    }

    printf("%d\n", num_elements);
    num_verts_ = num_elements / ELEMENTS_PER_VERT;
    modelFile.close();
}

int Model::NumElements() const {
    return num_verts_ * ELEMENTS_PER_VERT;
}

int Model::NumVerts() const {
    return num_verts_;
}

std::vector<glm::vec4> Model::Vertices() const {
    std::vector<glm::vec4> verts;
    for (int i = POSITION_OFFSET; i < NumElements(); i += ATTRIBUTE_STRIDE) {
        verts.push_back(glm::vec4(model_[i], model_[i + 1], model_[i + 2], 1.0));  // These are positions so w=1 for vec4s
    }

    return verts;
}
