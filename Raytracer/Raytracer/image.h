#pragma once

#include <cstdint>

#define COLOR_CHANNELS 3

struct Pixel
{
    uint8_t r, g, b;
};

class Image {
   public:
    union PixelData {
        uint8_t* raw;
        Pixel* pixels;
    };

    Image(int width, int height);
    ~Image();

    bool ValidCoord(int x, int y) const;
    void SetPixel(int x, int y, Pixel p);

    int Width();
    int Height();

    void Write(const char* name) const;

   private:
    PixelData data;
    int width_;
    int height_;
};
