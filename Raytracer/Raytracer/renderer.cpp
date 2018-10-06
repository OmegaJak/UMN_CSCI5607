#include "pch.h"
#include "renderer.h"
#include <iostream>

Renderer::Renderer(int width, int height, std::string filename)
    : render_width_(width), render_height_(height), output_filename_(filename) {}

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

void Renderer::SetBackgroundColor(Color background) {
#ifdef _DEBUG
    std::cout << "Background color set to " << background << "." << std::endl;
#endif
    background_color_ = background;
}

void Renderer::Render() {
    Image image = Image(render_width_, render_height_);
    Intersection intersection;
    for (int j = 0; j < render_height_; j++) {
        for (int i = 0; i < render_width_; i++) {
            Ray ray = scene_->GetCamera().ConstructRayThroughPixel(i, j, render_width_, render_height_);
            if (scene_->FindIntersection(ray, intersection)) {
                //std::cout << "X: " << i << ", Y: " << j << ", Ray: " << ray << std::endl;
                image.SetPixel(i, j, Pixel(scene_->GetColor(intersection.material, ray.start_point_, intersection.hit_point)));
            } else {
                image.SetPixel(i, j, Pixel(background_color_));
            }
        }
    }

    image.Write(output_filename_.c_str());
}
