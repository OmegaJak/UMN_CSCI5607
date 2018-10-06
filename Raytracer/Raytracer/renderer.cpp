#include "pch.h"
#include "renderer.h"
#include <iostream>

Renderer::Renderer(double width, double height, std::string filename)
    : render_width_(width), render_height_(height), output_filename_(filename) {}

Renderer::~Renderer() {
    delete scene_;
}

void Renderer::SetRenderDimensions(double width, double height) {
    render_width_ = width;
    render_height_ = height;
}

void Renderer::SetOutputFilename(std::string filename) {
    output_filename_ = filename;
}

void Renderer::SetScene(Scene *scene) {
    scene_ = scene;
    scene->SetCameraAspectRatio(render_width_ / render_height_);
}

void Renderer::Render() {
    Image image = Image(render_width_, render_height_);
    for (int j = 0; j < render_height_; j++) {
        for (int i = 0; i < render_width_; i++) {
            Ray ray = scene_->GetCamera().ConstructRayThroughPixel(i, j, render_width_, render_height_);
            if (scene_->FindIntersection(ray)) {
                //std::cout << "X: " << i << ", Y: " << j << ", Ray: " << ray << std::endl;
                image.SetPixel(i, j, Pixel{255, 255, 255});
            }
        }
    }

    image.Write(output_filename_.c_str());
}
