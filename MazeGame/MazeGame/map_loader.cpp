#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include "map_loader.h"

using std::cout;
using std::endl;
using std::string;

MapLoader::MapLoader() {
    LoadAssets();
}

MapLoader::~MapLoader() {}

std::vector<GameObject> MapLoader::LoadMap(const string& filename) {
    int width, height;
    char c;
    std::vector<GameObject> map_elements;

    std::fstream file(filename);
    if (file.fail()) {
        cout << "Failed to open file \"" << filename << "\". Exiting." << endl;
        exit(1);
    }

    file >> width >> height;

    string line;
    std::vector<string> lines;
    while (getline(file, line)) {
        if (line.length() == 0 || line.at(0) == '#') continue;

        cout << "Row " << lines.size() << ": " << line << endl;
        if (line.length() != width) {
            cout << "Row " << lines.size() << " of map had incorrect width of " << line.length() << endl;
            exit(1);
        }

        lines.push_back(line);
    }

    if (lines.size() != height) {
        cout << "Map had incorrect height of " << lines.size() << endl;
        exit(1);
    }

    if (!wall_model_ || !door_model_ || !key_model_) {
        cout << "Models failed to initialize for map. Exiting..." << endl;
        exit(1);
    }

    GameObject current_object;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            char current_char = lines[j][i];
            if (IsKey(current_char)) {
                current_object = GameObject(key_model_);
                current_object.Translate(glm::vec3(i, j, 1));
            } else if (IsDoor(current_char)) {
                current_object = GameObject(door_model_);
                current_object.Translate(glm::vec3(i, j, 1));
            } else {
                switch (current_char) {
                    case 'W':
                        current_object = GameObject(wall_model_);
                        current_object.Translate(glm::vec3(i, j, 1));
                        current_object.SetTextureIndex(TEX0);
                    case 'S':
                        break;
                    case 'G':
                        break;
                    case '0':
                        continue;
                    default:
                        break;
                }
            }

            current_object.material_ = GetMaterialForCharacter(current_char);
            map_elements.push_back(current_object);
        }
    }

    return map_elements;
}

Material MapLoader::GetMaterialForCharacter(char c) {
    switch (std::tolower(c)) {
        case 'a':
            return Material(1, 0.9, 0);
        case 'b':
            return Material(0.3, 0.5, 0.8);
        case 'c':
            return Material(0.5, 0.7, 0.1);
        case 'd':
            return Material(0.1, 0.2, 0.3);
        case 'e':
            return Material(0.8, 0.2, 0.8);
        case 's':
            return Material(0, 1, 1);
        case 'g':
            return Material(1, 1, 0);
        default:
            return Material();
    }
}

void MapLoader::LoadAssets() {
    wall_model_ = new Model("models/cube.txt");
    door_model_ = new Model("models/knot.txt");
    key_model_ = new Model("models/teapot.txt");
}

bool MapLoader::IsDoor(char c) {
    return c >= 65 && c <= 69;
}

bool MapLoader::IsKey(char c) {
    return c >= 97 && c <= 101;
}
