#include "pch.h"
#include "renderer.h"
#include <stdlib.h>
#include <chrono>
#include <iostream>
#include "image.h"
#include "time_keeper.h"

Renderer::Renderer(int width, int height, int max_recursive_depth, std::string filename)
    : render_width_(width), render_height_(height), max_recursive_depth_(max_recursive_depth), output_filename_(filename) {
    srand(time(NULL));
}

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

std::chrono::milliseconds Renderer::Render(const double num_status_updates, const int supersample_radius, bool jittered) {
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

            Color color = Color(0, 0, 0);
            for (int p = 0; p < supersample_radius; p++) {
                for (int q = 0; q < supersample_radius; q++) {
                    double r = jittered ? (rand() / double(RAND_MAX)) : 0.5;  // 0 to 1
                    Ray ray = scene_->GetCamera()->ConstructRayThroughPixel(i + (p + r) / supersample_radius,
                                                                           j + (q + r) / supersample_radius, render_width_, render_height_);
                    color += scene_->EvaluateRayTree(ray, max_recursive_depth_).Clamp();
                    /*int num_time_samples = 1;
                    Color tempColor = Color(0, 0, 0);
                    for (int time = 0; time < num_time_samples; time++) {
                        TimeKeeper::SetTime(rand() / double(RAND_MAX));
                        tempColor += scene_->EvaluateRayTree(ray, max_recursive_depth_).Clamp();
                    }
                    color += tempColor / double(num_time_samples);*/
                    intersection.ResetT();
                }
            }
            image_->SetPixel(i, j, (color / (supersample_radius * supersample_radius)).Clamp());
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
