#pragma once
#include <string>

class Model {
   public:
    explicit Model(const std::string& file);

    void Load(const std::string& file);

    int NumElements() const;
    int NumVerts() const;

    float* model_;
    int vbo_vertex_start_index_;

   private:
    int num_verts_;
};
