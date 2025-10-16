#include "Classes/configs/models/LevelConfig.h"

namespace cardgame {

LevelConfig::LevelConfig(int levelId,
                         std::vector<CardStaticData> playfieldCards,
                         std::vector<CardStaticData> stackCards)
    : _levelId(levelId)
    , _playfieldCards(std::move(playfieldCards))
    , _stackCards(std::move(stackCards)) {}

int LevelConfig::getLevelId() const { return _levelId; }

const std::vector<CardStaticData>& LevelConfig::getPlayfieldCards() const {
    return _playfieldCards;
}

const std::vector<CardStaticData>& LevelConfig::getStackCards() const {
    return _stackCards;
}

} // namespace cardgame
