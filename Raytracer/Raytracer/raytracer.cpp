#include "pch.h"
#include <iostream>
#include "parser.h"
#include "camera.h"

int main() {
    std::cout << "Hello World!\n";
    Parser parser;
    Renderer *renderer = parser.Parse("ambient_sphere.scn");
    renderer->Render();

    delete renderer;
}
