#include "Classes/configs/models/CardResConfig.h"

#include <utility>
#include <vector>

#include "cocos2d.h"

namespace cardgame {

CardResConfig::CardResConfig() {
    const std::vector<CardSuitType> suits = {
        CardSuitType::CST_CLUBS,
        CardSuitType::CST_DIAMONDS,
        CardSuitType::CST_HEARTS,
        CardSuitType::CST_SPADES};

    const std::vector<CardFaceType> faces = {
        CardFaceType::CFT_ACE,   CardFaceType::CFT_TWO,   CardFaceType::CFT_THREE,
        CardFaceType::CFT_FOUR,  CardFaceType::CFT_FIVE,  CardFaceType::CFT_SIX,
        CardFaceType::CFT_SEVEN, CardFaceType::CFT_EIGHT, CardFaceType::CFT_NINE,
        CardFaceType::CFT_TEN,   CardFaceType::CFT_JACK,  CardFaceType::CFT_QUEEN,
        CardFaceType::CFT_KING};

    for (CardSuitType suit : suits) {
        for (CardFaceType face : faces) {
            const std::string key = buildKey(suit, face);
            const std::string fileName = cocos2d::StringUtils::format(
                "res/cards/%s_%s.png", toString(suit).c_str(), toString(face).c_str());
            _cardResMap[key] = fileName;
        }
    }

    _cardResMap[buildKey(CardSuitType::CST_JOKER, CardFaceType::CFT_JOKER)] =
        "res/cards/joker.png";
}

std::string CardResConfig::getCardResPath(CardSuitType suit, CardFaceType face) const {
    const std::string key = buildKey(suit, face);
    auto iter = _cardResMap.find(key);
    if (iter != _cardResMap.end()) {
        return iter->second;
    }
    return "res/cards/default.png";
}

std::string CardResConfig::buildKey(CardSuitType suit, CardFaceType face) {
    return toString(suit) + "_" + toString(face);
}

} // namespace cardgame
