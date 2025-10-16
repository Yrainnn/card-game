#include "Classes/models/CardModel.h"

namespace cardgame {

CardModel::CardModel(int cardId,
                     CardFaceType faceType,
                     CardSuitType suitType,
                     const cocos2d::Vec2& pos,
                     CardAreaType area,
                     bool isFlipped,
                     bool isUsed)
    : _cardId(cardId)
    , _faceType(faceType)
    , _suitType(suitType)
    , _currentPos(pos)
    , _isFlipped(isFlipped)
    , _currentArea(area)
    , _isUsed(isUsed) {}

int CardModel::getCardId() const { return _cardId; }

CardFaceType CardModel::getFaceType() const { return _faceType; }

CardSuitType CardModel::getSuitType() const { return _suitType; }

const cocos2d::Vec2& CardModel::getCurrentPos() const { return _currentPos; }

bool CardModel::isFlipped() const { return _isFlipped; }

CardAreaType CardModel::getCurrentArea() const { return _currentArea; }

bool CardModel::isUsed() const { return _isUsed; }

void CardModel::setCurrentPos(const cocos2d::Vec2& pos) { _currentPos = pos; }

void CardModel::setIsFlipped(bool flipped) { _isFlipped = flipped; }

void CardModel::setCurrentArea(CardAreaType area) { _currentArea = area; }

void CardModel::setIsUsed(bool used) { _isUsed = used; }

} // namespace cardgame
