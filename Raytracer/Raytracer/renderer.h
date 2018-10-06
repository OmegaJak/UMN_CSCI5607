#pragma once
#include <string>
#include "scene.h"

class Renderer {
   public:
    Renderer(int width = 640, int height = 480, std::string filename = "raytraced.bmp");
    ~Renderer();

    void SetRenderDimensions(int width, int height);
    void SetOutputFilename(std::string filename);
    void SetScene(Scene *scene);
    void SetBackgroundColor(Color background);

    void Render();

private:
    int render_width_;
    int render_height_;
    std::string output_filename_;
    Scene *scene_;
    Color background_color_;
};
