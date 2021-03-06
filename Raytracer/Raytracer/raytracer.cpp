#include "pch.h"
#include <iostream>
#include <map>
#include "parser.h"

std::chrono::milliseconds ParseAndRenderScene(std::string scene_file, double num_status_updates, bool should_output) {
    std::cout << "-----------------------------" << std::endl;
    std::cout << "Beginning Parsing of \"" << scene_file << "\"" << std::endl;
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

int main(int argc, char*argv[]) {
    std::string default_scene = "spheres2.scn";
    std::string scene_file;
    if (argc != 2) { 
        printf("No scene file was specified to run. Running default of %s\n", default_scene.c_str());
        scene_file = default_scene;
    } else {
        scene_file = std::string(argv[1]);
    }

    if (scene_file == "gamut") {
        RunTheGamut();
    } else {
        ParseAndRenderScene(scene_file, 20, true);
    }
}
