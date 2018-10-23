#include "pch.h"
#include <iostream>
#include "parser.h"

int main() {
    std::cout << "Hello World!\n";
    Parser parser;
    Renderer *renderer = parser.Parse("plane.scn");
    renderer->Render();

    delete renderer;
}
