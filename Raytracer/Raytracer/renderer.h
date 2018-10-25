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
    void SetDOF(double focal_distance, double lens_radius, int num_samples);
    void SetSuperSampleRadius(int radius);
    void SetJitteredSupersampling(bool jittered);
    void SetMotionWarp(bool motion_warp);

    std::chrono::milliseconds Render(const double num_status_updates);
    void OutputImage() const;

   private:
    int render_width_;
    int render_height_;
    int max_recursive_depth_;
    double focal_distance_, lens_radius_, sqr_supersample_rad_ = 1;
    int dof_samples_ = 1, supersample_radius_ = 1;
    bool jittered_ = false, motion_warp_ = false;
    std::string output_filename_;
    Image *image_ = nullptr;
    Scene *scene_;
};
