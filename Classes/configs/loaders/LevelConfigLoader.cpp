#include "Classes/configs/loaders/LevelConfigLoader.h"

#include <vector>

#include "Classes/utils/JsonUtils.h"
#include "Classes/utils/LogUtils.h"

namespace cardgame {

LevelConfig LevelConfigLoader::loadLevelConfig(int levelId) {
    const std::string filePath = cocos2d::StringUtils::format("configs/levels/level_%d.json", levelId);
    cocos2d::Value root = JsonUtils::readJsonFile(filePath);

    std::vector<CardStaticData> playfieldCards;
    std::vector<CardStaticData> stackCards;

    if (root.getType() != cocos2d::Value::Type::MAP) {
        LogUtils::logError("LevelConfigLoader", "Invalid level config json: " + filePath);
        return LevelConfig(levelId, std::move(playfieldCards), std::move(stackCards));
    }

    const auto& rootMap = root.asValueMap();

    auto playfieldIter = rootMap.find("playfield");
    if (playfieldIter != rootMap.end() && playfieldIter->second.getType() == cocos2d::Value::Type::VECTOR) {
        const auto& vec = playfieldIter->second.asValueVector();
        playfieldCards.reserve(vec.size());
        for (const auto& value : vec) {
            if (value.getType() != cocos2d::Value::Type::MAP) {
                continue;
            }
            const auto& cardMap = value.asValueMap();
            int faceVal = JsonUtils::getIntFromJson(cardMap, "face");
            int suitVal = JsonUtils::getIntFromJson(cardMap, "suit");
            cocos2d::Vec2 initPos = JsonUtils::getVec2FromJson(cardMap, "x", "y");

            playfieldCards.push_back({parseFaceType(faceVal), parseSuitType(suitVal), initPos});
        }
    }

    auto stackIter = rootMap.find("stack");
    if (stackIter != rootMap.end() && stackIter->second.getType() == cocos2d::Value::Type::VECTOR) {
        const auto& vec = stackIter->second.asValueVector();
        stackCards.reserve(vec.size());
        for (const auto& value : vec) {
            if (value.getType() != cocos2d::Value::Type::MAP) {
                continue;
            }
            const auto& cardMap = value.asValueMap();
            int faceVal = JsonUtils::getIntFromJson(cardMap, "face");
            int suitVal = JsonUtils::getIntFromJson(cardMap, "suit");
            stackCards.push_back({parseFaceType(faceVal), parseSuitType(suitVal), cocos2d::Vec2::ZERO});
        }
    }

    return LevelConfig(levelId, std::move(playfieldCards), std::move(stackCards));
}

CardFaceType LevelConfigLoader::parseFaceType(int faceVal) {
    switch (faceVal) {
    case 1:
        return CardFaceType::CFT_ACE;
    case 2:
        return CardFaceType::CFT_TWO;
    case 3:
        return CardFaceType::CFT_THREE;
    case 4:
        return CardFaceType::CFT_FOUR;
    case 5:
        return CardFaceType::CFT_FIVE;
    case 6:
        return CardFaceType::CFT_SIX;
    case 7:
        return CardFaceType::CFT_SEVEN;
    case 8:
        return CardFaceType::CFT_EIGHT;
    case 9:
        return CardFaceType::CFT_NINE;
    case 10:
        return CardFaceType::CFT_TEN;
    case 11:
        return CardFaceType::CFT_JACK;
    case 12:
        return CardFaceType::CFT_QUEEN;
    case 13:
        return CardFaceType::CFT_KING;
    case 14:
        return CardFaceType::CFT_JOKER;
    default:
        return CardFaceType::CFT_ACE;
    }
}

CardSuitType LevelConfigLoader::parseSuitType(int suitVal) {
    switch (suitVal) {
    case 0:
        return CardSuitType::CST_CLUBS;
    case 1:
        return CardSuitType::CST_DIAMONDS;
    case 2:
        return CardSuitType::CST_HEARTS;
    case 3:
        return CardSuitType::CST_SPADES;
    case 4:
        return CardSuitType::CST_JOKER;
    default:
        return CardSuitType::CST_CLUBS;
    }
}

} // namespace cardgame
