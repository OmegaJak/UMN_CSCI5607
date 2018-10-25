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

void Renderer::SetDOF(double focal_distance, double lens_radius, int num_samples) {
    focal_distance_ = focal_distance;
    lens_radius_ = lens_radius;
    dof_samples_ = num_samples;
}

void Renderer::SetSuperSampleRadius(int radius) {
    supersample_radius_ = radius;
    sqr_supersample_rad_ = supersample_radius_ * supersample_radius_;
}

void Renderer::SetJitteredSupersampling(bool jittered) {
    jittered_ = jittered;
}

void Renderer::SetMotionWarp(bool motion_warp) {
    motion_warp_ = motion_warp;
}

std::chrono::milliseconds Renderer::Render(const double num_status_updates) {
    delete image_;
    image_ = new Image(render_width_, render_height_);

    auto start = std::chrono::high_resolution_clock::now();
    Intersection intersection;
    int total_pixels = render_width_ * render_height_;
    int current_status = 0;
    double ray_i, ray_j;
    std::vector<Ray> rays;
    for (int j = 0; j < render_height_; j++) {
        for (int i = 0; i < render_width_; i++) {
            int current_pixel = j * render_width_ + i;
            if (motion_warp_) {
                TimeKeeper::SetTime(current_pixel / double(total_pixels));
            }
            if (num_status_updates != 0 && round((current_status / num_status_updates) * total_pixels) == current_pixel) {
                std::cout << (current_status / num_status_updates) * 100 << "%" << std::endl;
                current_status += 1;
            }

            Color color = Color(0, 0, 0);
            for (int p = 0; p < supersample_radius_; p++) {
                for (int q = 0; q < supersample_radius_; q++) {
                    if (supersample_radius_ > 1) {
                        double r = jittered_ ? (rand() / double(RAND_MAX)) : 0.5;  // 0 to 1
                        ray_i = i + (p + r) / supersample_radius_;
                        ray_j = j + (q + r) / supersample_radius_;
                    } else {
                        ray_i = i + 0.5;
                        ray_j = j + 0.5;
                    }

                    if (dof_samples_ == 1) {
                        Ray ray = scene_->GetCamera()->ConstructRayThroughPixel(ray_i, ray_j, render_width_, render_height_);
                        color += scene_->EvaluateRayTree(ray, max_recursive_depth_).Clamp();
                    } else {
                        Color temp_color = Color(0, 0, 0);
                        rays = scene_->GetCamera()->ConstructRaysThroughPixel(ray_i, ray_j, render_width_, render_height_, focal_distance_, lens_radius_, dof_samples_);
                        for (Ray ray : rays) {
                            temp_color += scene_->EvaluateRayTree(ray, max_recursive_depth_).Clamp();
                        }
                        color += temp_color / dof_samples_;
                    }

                    //color += scene_->EvaluateRayTree(ray, max_recursive_depth_).Clamp();
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
            image_->SetPixel(i, j, (color / double(sqr_supersample_rad_)).Clamp());
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
