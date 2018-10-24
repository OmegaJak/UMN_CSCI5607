#include "pch.h"
#include "renderer.h"
#include <chrono>
#include <iostream>
#include "image.h"
#include <chrono>

Renderer::Renderer(int width, int height, int max_recursive_depth, std::string filename)
    : render_width_(width),
      render_height_(height),
      max_recursive_depth_(max_recursive_depth),
      output_filename_(filename) {}

Renderer::~Renderer() {
    delete image_;
    delete scene_;
}

void Renderer::SetRenderDimensions(int width, int height) {
    render_width_ = width;
    render_height_ = height;
}

void Renderer::SetOutputFilename(std::string filename) {
    output_filename_ = filename;
}

void Renderer::SetScene(Scene *scene) {
    scene_ = scene;
    scene->SetCameraAspectRatio(render_width_ / double(render_height_));
}

void Renderer::SetRecursiveDepth(int recursive_depth) {
    max_recursive_depth_ = recursive_depth;
}

std::chrono::milliseconds Renderer::Render(const double num_status_updates) {
    delete image_;
    image_ = new Image(render_width_, render_height_);

    auto start = std::chrono::high_resolution_clock::now();
    Intersection intersection;
    int total_pixels = render_width_ * render_height_;
    int current_status = 0;
    for (int j = 0; j < render_height_; j++) {
        for (int i = 0; i < render_width_; i++) {
            int current_pixel = j * render_width_ + i;
            if (num_status_updates != 0 && round((current_status / num_status_updates) * total_pixels) == current_pixel) {
                std::cout << (current_status / num_status_updates) * 100 << "%" << std::endl;
                current_status += 1;
            }
            Ray ray = scene_->GetCamera().ConstructRayThroughPixel(i, j, render_width_, render_height_);
            image_->SetPixel(i, j, scene_->EvaluateRayTree(ray, max_recursive_depth_).Clamp());
            intersection.ResetT();
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto run_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "100%" << std::endl;

    return run_time;
}

void Renderer::OutputImage() const {
    image_->Write(output_filename_.c_str());
}
