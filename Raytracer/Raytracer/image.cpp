#include "pch.h"
#include "image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION  // only place once in one .cpp files
#include "../../HW1/src/stb_image_write.h"

Image::Image() {}

Image::Image(int width, int height) {
    width_ = width;
    height_ = height;
    data.raw = new uint8_t[width_ * height_ * COLOR_CHANNELS];
    for (int i = 0; i < width_ * height_ * COLOR_CHANNELS; i++) {
        data.raw[i] = 255;
    }
}

Image::~Image() {
    delete[] data.raw;
}

void Image::Write(const char* filename) const {
    stbi_write_bmp(filename, width_, height_, 3, data.raw);
}
