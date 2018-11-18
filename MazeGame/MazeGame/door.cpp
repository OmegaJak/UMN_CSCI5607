#include "door.h"

Door::Door(Model* model, char id) : GameObject(model) {
    id_ = id;
}

bool Door::MatchesId(char id) {
    return tolower(id) == tolower(id_);
}

void Door::GoAway() {
    transform->ResetAndSetTranslation(glm::vec3(0, 0, -3));
}
