#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include "goal.h"
#include "map.h"
#include "map_loader.h"
#include "spawn.h"
#include "wall.h"

using std::cout;
using std::endl;
using std::string;

MapLoader::MapLoader() {
    LoadAssets();
}

MapLoader::~MapLoader() {}

Map* MapLoader::LoadMap(const string& filename) {
    int width, height;
    Map* map = new Map();

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
    map->Add(ground);

    GameObject* current_object;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            char current_char = lines[j][i];
            glm::vec3 base_position = GetPositionForCoordinate(i, j);
            if (IsKey(current_char)) {
                current_object = new Key(key_model_, map);
                current_object->transform->Translate(base_position);
            } else if (IsDoor(current_char)) {
                current_object = new Door(door_model_);
                current_object->transform->Translate(base_position);
            } else {
                switch (current_char) {
                    case 'W':
                        current_object = new Wall(wall_model_);
                        current_object->transform->Translate(base_position);
                        current_object->SetTextureIndex(TEX0);
                        break;
                    case 'S':
                        current_object = new Spawn(start_model_);
                        current_object->transform->Translate(base_position);
                        break;
                    case 'G':
                        current_object = new Goal(goal_model_);
                        current_object->transform->Translate(base_position);
                        break;
                    case '0':
                        continue;
                    default:
                        printf("Unrecognized character \'%c\'", current_char);
                        continue;
                }
            }

            current_object->material = GetMaterialForCharacter(current_char);
            map->Add(current_object);
        }
    }

    return map;
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
    start_model_ = goal_model_ = key_model_;
}

glm::vec3 MapLoader::GetPositionForCoordinate(int i, int j) {
    return glm::vec3(i, j, 0) + glm::vec3(0.5);
}

bool MapLoader::IsDoor(char c) {
    return c >= 65 && c <= 69;
}

bool MapLoader::IsKey(char c) {
    return c >= 97 && c <= 101;
}
