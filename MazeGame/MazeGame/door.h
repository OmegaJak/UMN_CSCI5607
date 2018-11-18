#pragma once
#include "game_object.h"

class Door : public GameObject {
   public:
    Door(Model* model, char id);
    ~Door() = default;

    bool IsSolid() override {
        return true;
    }

    bool MatchesId(char id);
    void GoAway();

   private:
    char id_;
};
