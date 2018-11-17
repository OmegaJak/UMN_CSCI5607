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

std::vector<GameObject*> MapLoader::LoadMap(const string& filename) {
    int width, height;
    std::vector<GameObject*> map_elements;

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

    GameObject* ground = new GameObject(wall_model_);
    ground->transform->Scale(glm::vec3(1, 1, 0));
    ground->transform->Translate(glm::vec3((width / 2) + 0.5, (height / 2) + 0.5, 0));
    ground->transform->Scale(glm::vec3(width, height, 1));
    ground->material.color_ = glm::vec3(0.8, 0.8, 0.8);
    map_elements.push_back(ground);

    GameObject* current_object;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            char current_char = lines[j][i];
            if (IsKey(current_char)) {
                current_object = new GameObject(key_model_);
                current_object->transform->Translate(glm::vec3(i + 0.5f, j + 0.5f, 0.5));
                current_object->material = GetMaterialForCharacter(current_char);
                map_elements.push_back(current_object);
            } else if (IsDoor(current_char)) {
                current_object = new GameObject(door_model_);
                current_object->transform->Translate(glm::vec3(i + 0.5f, j + 0.5f, 0.5));
                current_object->material = GetMaterialForCharacter(current_char);
                map_elements.push_back(current_object);
            } else {
                switch (current_char) {
                    case 'W':
                        current_object = new GameObject(wall_model_);
                        current_object->transform->Translate(glm::vec3(i + 0.5f, j + 0.5f, 0.5));
                        current_object->SetTextureIndex(TEX0);
                        current_object->material = GetMaterialForCharacter(current_char);
                        map_elements.push_back(current_object);
                        break;
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
        }
    }

    return map_elements;
}

Material MapLoader::GetMaterialForCharacter(char c) {
    switch (std::tolower(c)) {
        case 'a':
            return Material(1, 0.9f, 0);
        case 'b':
            return Material(0.3f, 0.5f, 0.8f);
        case 'c':
            return Material(0.5f, 0.7f, 0.1f);
        case 'd':
            return Material(0.1f, 0.2f, 0.3f);
        case 'e':
            return Material(0.8f, 0.2f, 0.8f);
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
