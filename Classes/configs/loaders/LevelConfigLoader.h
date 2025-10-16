#pragma once

#include <string>

#include "Classes/configs/models/LevelConfig.h"

namespace cardgame {

class LevelConfigLoader {
public:
    static LevelConfig loadLevelConfig(int levelId);

private:
    static CardFaceType parseFaceType(int faceVal);
    static CardSuitType parseSuitType(int suitVal);
};

} // namespace cardgame
