#pragma once

#include "Classes/models/CardEnums.h"

namespace cardgame {

enum class CardAreaType {
    AREA_NONE = -1,
    AREA_PLAYFIELD = 0,
    AREA_STACK,
    AREA_TRAY
};

class CardModel {
public:
    CardModel() = default;
    CardModel(int cardId,
              CardFaceType faceType,
              CardSuitType suitType,
              const cocos2d::Vec2& pos,
              CardAreaType area,
              bool isFlipped,
              bool isUsed);

    int getCardId() const;
    CardFaceType getFaceType() const;
    CardSuitType getSuitType() const;
    const cocos2d::Vec2& getCurrentPos() const;
    bool isFlipped() const;
    CardAreaType getCurrentArea() const;
    bool isUsed() const;

    void setCurrentPos(const cocos2d::Vec2& pos);
    void setIsFlipped(bool flipped);
    void setCurrentArea(CardAreaType area);
    void setIsUsed(bool used);

private:
    int _cardId = -1;
    CardFaceType _faceType = CardFaceType::CFT_ACE;
    CardSuitType _suitType = CardSuitType::CST_CLUBS;
    cocos2d::Vec2 _currentPos = cocos2d::Vec2::ZERO;
    bool _isFlipped = false;
    CardAreaType _currentArea = CardAreaType::AREA_NONE;
    bool _isUsed = false;
};

} // namespace cardgame
