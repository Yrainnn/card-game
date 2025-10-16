#pragma once

#include "cocos2d.h"

namespace cardgame {

enum class CardFaceType {
    CFT_ACE = 1,
    CFT_TWO,
    CFT_THREE,
    CFT_FOUR,
    CFT_FIVE,
    CFT_SIX,
    CFT_SEVEN,
    CFT_EIGHT,
    CFT_NINE,
    CFT_TEN,
    CFT_JACK,
    CFT_QUEEN,
    CFT_KING,
    CFT_JOKER
};

enum class CardSuitType {
    CST_CLUBS = 0,
    CST_DIAMONDS,
    CST_HEARTS,
    CST_SPADES,
    CST_JOKER
};

inline std::string toString(CardFaceType face) {
    switch (face) {
    case CardFaceType::CFT_ACE:
        return "CFT_ACE";
    case CardFaceType::CFT_TWO:
        return "CFT_TWO";
    case CardFaceType::CFT_THREE:
        return "CFT_THREE";
    case CardFaceType::CFT_FOUR:
        return "CFT_FOUR";
    case CardFaceType::CFT_FIVE:
        return "CFT_FIVE";
    case CardFaceType::CFT_SIX:
        return "CFT_SIX";
    case CardFaceType::CFT_SEVEN:
        return "CFT_SEVEN";
    case CardFaceType::CFT_EIGHT:
        return "CFT_EIGHT";
    case CardFaceType::CFT_NINE:
        return "CFT_NINE";
    case CardFaceType::CFT_TEN:
        return "CFT_TEN";
    case CardFaceType::CFT_JACK:
        return "CFT_JACK";
    case CardFaceType::CFT_QUEEN:
        return "CFT_QUEEN";
    case CardFaceType::CFT_KING:
        return "CFT_KING";
    case CardFaceType::CFT_JOKER:
        return "CFT_JOKER";
    }
    return "UNKNOWN_FACE";
}

inline std::string toString(CardSuitType suit) {
    switch (suit) {
    case CardSuitType::CST_CLUBS:
        return "CST_CLUBS";
    case CardSuitType::CST_DIAMONDS:
        return "CST_DIAMONDS";
    case CardSuitType::CST_HEARTS:
        return "CST_HEARTS";
    case CardSuitType::CST_SPADES:
        return "CST_SPADES";
    case CardSuitType::CST_JOKER:
        return "CST_JOKER";
    }
    return "UNKNOWN_SUIT";
}

} // namespace cardgame
