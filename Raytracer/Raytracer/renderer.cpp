#include "pch.h"
#include "renderer.h"
#include <iostream>
#include "image.h"

Renderer::Renderer(int width, int height, int max_recursive_depth, std::string filename)
    : render_width_(width), render_height_(height), max_recursive_depth_(max_recursive_depth), output_filename_(filename) {}

Renderer::~Renderer() {
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

void Renderer::Render() {
    Image image = Image(render_width_, render_height_);
    Intersection intersection;
    for (int j = 0; j < render_height_; j++) {
        for (int i = 0; i < render_width_; i++) {
            Ray ray = scene_->GetCamera().ConstructRayThroughPixel(i, j, render_width_, render_height_);
            image.SetPixel(i, j, scene_->EvaluateRayTree(ray, max_recursive_depth_).Clamp());
        }
    }

    image.Write(output_filename_.c_str());
}
