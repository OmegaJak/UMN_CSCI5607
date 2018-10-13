#include "pch.h"
#include <iostream>
#include "parser.h"
#include <chrono>

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Hello World!\n";
    Parser parser;
    Renderer *renderer = parser.Parse("spheres2.scn");
    renderer->Render();
    auto end = std::chrono::high_resolution_clock::now();

    auto run_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "-----------------------------" << std::endl;
    std::cout << "Runtime : " << run_time.count() << "ms" << std::endl;

    delete renderer;
}
