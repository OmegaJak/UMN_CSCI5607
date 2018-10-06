#pragma once

#include <cstdint>
#include "color.h"

#define COLOR_CHANNELS 3

struct Pixel {
    uint8_t r, g, b;

    int Clamp(int n) {
        if (n < 0) {
            return 0;
        }
        if (n > 255) {
            return 255;
        }

        return n;
    }

    Pixel(int _r, int _g, int _b) {
        r = Clamp(_r);
        g = Clamp(_g);
        b = Clamp(_b);
    }

    Pixel(const Color& color) : Pixel(round(color.red_ * 255), round(color.green_ * 255), round(color.blue_ * 255)) {}
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
