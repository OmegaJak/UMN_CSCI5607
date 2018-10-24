#include "pch.h"
#include <iostream>
#include <map>
#include "Rectangle.h"
#include "parser.h"
#include "rectangular_prism.h"
#include "sphere.h"
#include "triangle.h"

std::chrono::milliseconds ParseAndRenderScene(std::string scene_file, double num_status_updates, bool should_output) {
    std::cout << "-----------------------------" << std::endl;
    std::cout << "Beginning Parsing" << std::endl;
    Parser parser;
    Renderer *renderer = parser.Parse(scene_file);
    std::cout << "Finished Parsing" << std::endl;
    std::chrono::milliseconds runtime = renderer->Render(num_status_updates);
    std::cout << "-----------------------------" << std::endl;
    std::cout << "Runtime : " << runtime.count() << "ms" << std::endl;

    if (should_output) renderer->OutputImage();
    delete renderer;

    return runtime;
}

void RunTheGamut() {
    std::string gamut_scenes[] = {"spheres1.scn", "spheres2.scn",     "triangle.scn",  "plane.scn",  "test_reasonable.scn",
                                  "bear.scn",     "triangle_his.scn", "spotlight.scn", "outdoor.scn"};
    std::map<std::string, std::chrono::milliseconds> runtimes;
    for (std::string scene : gamut_scenes) {
        std::chrono::milliseconds current_runtime = ParseAndRenderScene(scene, 4, false);
        runtimes[scene] = current_runtime;
    }

    std::cout << "-----------------------------" << std::endl;
    std::cout << "Runtimes: " << std::endl;
    for (auto runtime : runtimes) {
        std::cout << "\"" << runtime.first << "\": " << runtime.second.count() << "ms" << std::endl;
    }
}

int main() {
    std::cout << "Hello World!\n";
    ParseAndRenderScene("spheres2.scn", 4, true);
    // RunTheGamut();
    /*Rectangle testrect = Rectangle(Vector3(0, 0, 0), Vector3(20, 0, 0), Vector3(0, 10, 10));
    testrect.GenerateBoundingBox();

    RectangularPrism testrectp = RectangularPrism(Vector3(10, 10, 10), Vector3(10, 0, 0), Vector3(0, 0, 5), Vector3(0, 3, 0));
    testrectp.GenerateBoundingBox();

    Sphere testsphere = Sphere(Vector3(5, 5, 5), 5);
    testsphere.GenerateBoundingBox();

    Triangle testtri = Triangle(Vector3(-200, 1000, 50), Vector3(5000, 0, 2), Vector3(30, -200, 9000));
    testtri.GenerateBoundingBox();

    IntersectableGroup group = IntersectableGroup();
    group.AddChild(&testrect);
    group.AddChild(&testrectp);
    group.AddChild(&testsphere);
    group.AddChild(&testtri);
    group.GenerateBoundingBox();*/
}
