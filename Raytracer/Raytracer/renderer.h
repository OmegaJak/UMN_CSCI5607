#pragma once
#include <string>
#include "scene.h"

class Renderer {
   public:
    Renderer(double width = 640, double height = 480, std::string filename = "raytraced.bmp");
    ~Renderer();

    void SetRenderDimensions(double width, double height);
    void SetOutputFilename(std::string filename);
    void SetScene(Scene *scene);

    void Render();

private:
    double render_width_;
    double render_height_;
    std::string output_filename_;
    Scene *scene_;
};
