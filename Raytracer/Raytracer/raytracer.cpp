#include "pch.h"
#include <iostream>
#include "image.h"
#include "parser.h"
#include "ray.h"
#include "camera.h"

void WriteImage(int width, int height) {
    Image image = Image(50, 50);
    image.Write("test.bmp");
}

int main() {
    std::cout << "Hello World!\n";
    Parser parser;
    Scene scene = parser.Parse("ambient_sphere.scn");
    Camera camera = Camera(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0), 30, 30, 1);
    Image image = Image(200, 200);
    for (int j = 0; j < image.Height(); j++) {
        for (int i = 0; i < image.Width(); i++) {
            Ray ray = camera.ConstructRayThroughPixel(i, j, image.Width(), image.Height());
            if (scene.FindIntersection(ray)) {
                image.SetPixel(i, j, Pixel{ 255, 255, 255 });
            }
        }
    }

    image.Write("test.bmp");
}
