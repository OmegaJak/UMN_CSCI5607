#pragma once
#include <vector>
#include "game_object.h"
#include "map.h"
#include "material.h"
#include "model.h"

class MapLoader {
   public:
    MapLoader();
    ~MapLoader();

    std::vector<GameObject*> LoadMap(const std::string& filename);

   private:
    static Material GetMaterialForCharacter(char c);
    void LoadAssets();

    static bool IsDoor(char c);
    static bool IsKey(char c);

    Model* wall_model_ = nullptr;
    Model* door_model_ = nullptr;
    Model* key_model_ = nullptr;
};
