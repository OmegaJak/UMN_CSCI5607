#include "pch.h"
#include <iostream>
#include "image.h"
#include "parser.h"

void WriteImage(int width, int height) {
    Image image = Image(50, 50);
    image.Write("test.bmp");
}

int main() {
    std::cout << "Hello World!\n";
    Parser parser;
    Scene scene = parser.Parse("ambient_sphere.scn");
    WriteImage(50, 50);
}
