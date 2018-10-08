#include "pch.h"
#include <iostream>
#include "parser.h"

int main() {
    std::cout << "Hello World!\n";
    Parser parser;
    Renderer *renderer = parser.Parse("spheres1.scn");
    renderer->Render();

    delete renderer;
}
