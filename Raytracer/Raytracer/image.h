#pragma once

#include <cstdint>

#define COLOR_CHANNELS 3

class Image {
   public:
    union PixelData {
        uint8_t* raw;
    };

    Image();
    Image(int width, int height);
    ~Image();
    void Write(const char* name) const;

   private:
    PixelData data;
    int width_;
    int height_;
};
