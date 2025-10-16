#pragma once

#include <vector>

#include "cocos2d.h"

#include "Classes/models/CardEnums.h"

namespace cardgame {

struct CardStaticData {
    CardFaceType faceType;
    CardSuitType suitType;
    cocos2d::Vec2 initPos;
};

class LevelConfig {
public:
    LevelConfig() = default;
    LevelConfig(int levelId,
                std::vector<CardStaticData> playfieldCards,
                std::vector<CardStaticData> stackCards);

    int getLevelId() const;
    const std::vector<CardStaticData>& getPlayfieldCards() const;
    const std::vector<CardStaticData>& getStackCards() const;

private:
    int _levelId = 0;
    std::vector<CardStaticData> _playfieldCards;
    std::vector<CardStaticData> _stackCards;
};

} // namespace cardgame
