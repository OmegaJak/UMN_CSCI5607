#pragma once
#include <string>
#include "scene.h"

class Renderer {
   public:
    Renderer(int width = 640, int height = 480, int max_recursive_depth = 5, std::string filename = "raytraced.bmp");
    ~Renderer();

    void SetRenderDimensions(int width, int height);
    void SetOutputFilename(std::string filename);
    void SetScene(Scene *scene);
    void SetRecursiveDepth(int recursive_depth);

    void Render();

private:
    int render_width_;
    int render_height_;
    int max_recursive_depth_;
    std::string output_filename_;
    Scene *scene_;
};
