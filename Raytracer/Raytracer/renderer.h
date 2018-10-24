#pragma once
#include <chrono>
#include <string>
#include "image.h"
#include "scene.h"

class Renderer {
   public:
    Renderer(int width = 640, int height = 480, int max_recursive_depth = 5, std::string filename = "raytraced.bmp");
    ~Renderer();

    void SetRenderDimensions(int width, int height);
    void SetOutputFilename(std::string filename);
    void SetScene(Scene *scene);
    void SetRecursiveDepth(int recursive_depth);

    std::chrono::milliseconds Render(const double num_status_updates);
    void OutputImage() const;

   private:
    int render_width_;
    int render_height_;
    int max_recursive_depth_;
    std::string output_filename_;
    Image *image_ = nullptr;
    Scene *scene_;
};
