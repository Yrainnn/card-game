#pragma once

#include "Classes/configs/models/LevelConfig.h"
#include "Classes/models/GameModel.h"

namespace cardgame {

class GameModelGeneratorService {
public:
    static GameModel generateGameModel(const LevelConfig& levelConfig, int initTrayCardId = -1);
};

} // namespace cardgame
