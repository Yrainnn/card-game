#include "Classes/models/GameModel.h"

#include <sstream>
#include <utility>

#include "Classes/utils/JsonUtils.h"
#include "Classes/utils/LogUtils.h"

namespace cardgame {

CardModel& GameModel::getCardById(int cardId) { return _allCards.at(cardId); }

const CardModel& GameModel::getCardById(int cardId) const { return _allCards.at(cardId); }

void GameModel::addCard(CardModel cardModel) {
    const int cardId = cardModel.getCardId();
    _allCards[cardId] = std::move(cardModel);
}

const std::unordered_map<int, CardModel>& GameModel::getAllCards() const { return _allCards; }

int GameModel::getCurrentTrayCardId() const { return _currentTrayCardId; }

void GameModel::setCurrentTrayCardId(int cardId) { _currentTrayCardId = cardId; }

int GameModel::getLevelId() const { return _levelId; }

void GameModel::setLevelId(int levelId) { _levelId = levelId; }

bool GameModel::isGameOver() const { return _isGameOver; }

void GameModel::setGameOver(bool gameOver) { _isGameOver = gameOver; }

const std::vector<int>& GameModel::getStackCardIds() const { return _stackCardIds; }

void GameModel::setStackCardIds(const std::vector<int>& ids) { _stackCardIds = ids; }

void GameModel::removeStackTopCard() {
    if (!_stackCardIds.empty()) {
        _stackCardIds.erase(_stackCardIds.begin());
    }
}

void GameModel::pushStackCard(int cardId) { _stackCardIds.insert(_stackCardIds.begin(), cardId); }

size_t GameModel::getStackCardCount() const { return _stackCardIds.size(); }

void GameModel::markCardUsed(int cardId) { _allCards[cardId].setIsUsed(true); }

std::string GameModel::toJson() const {
    cocos2d::ValueMap root;
    root["levelId"] = _levelId;
    root["currentTrayCardId"] = _currentTrayCardId;
    root["isGameOver"] = _isGameOver;

    cocos2d::ValueVector stackVec;
    stackVec.reserve(_stackCardIds.size());
    for (int id : _stackCardIds) {
        stackVec.emplace_back(id);
    }
    root["stack"] = stackVec;

    cocos2d::ValueVector cardsVec;
    cardsVec.reserve(_allCards.size());
    for (const auto& entry : _allCards) {
        const CardModel& card = entry.second;
        cocos2d::ValueMap cardMap;
        cardMap["cardId"] = card.getCardId();
        cardMap["face"] = static_cast<int>(card.getFaceType());
        cardMap["suit"] = static_cast<int>(card.getSuitType());
        cardMap["posX"] = card.getCurrentPos().x;
        cardMap["posY"] = card.getCurrentPos().y;
        cardMap["area"] = static_cast<int>(card.getCurrentArea());
        cardMap["isFlipped"] = card.isFlipped();
        cardMap["isUsed"] = card.isUsed();
        cardsVec.emplace_back(cardMap);
    }
    root["cards"] = cardsVec;

    return JsonUtils::valueMapToJsonString(root);
}

void GameModel::fromJson(const std::string& jsonStr) {
    cocos2d::Value root = JsonUtils::parseJsonString(jsonStr);
    if (root.getType() != cocos2d::Value::Type::MAP) {
        LogUtils::logError("GameModel", "Failed to parse game model json string");
        return;
    }

    const auto& rootMap = root.asValueMap();
    auto levelIt = rootMap.find("levelId");
    if (levelIt != rootMap.end()) {
        _levelId = levelIt->second.asInt();
    }
    auto trayIt = rootMap.find("currentTrayCardId");
    if (trayIt != rootMap.end()) {
        _currentTrayCardId = trayIt->second.asInt();
    }
    auto gameOverIt = rootMap.find("isGameOver");
    if (gameOverIt != rootMap.end()) {
        _isGameOver = gameOverIt->second.asBool();
    }

    _stackCardIds.clear();
    auto stackIt = rootMap.find("stack");
    if (stackIt != rootMap.end() && stackIt->second.getType() == cocos2d::Value::Type::VECTOR) {
        for (const auto& val : stackIt->second.asValueVector()) {
            _stackCardIds.push_back(val.asInt());
        }
    }

    _allCards.clear();
    auto cardsIt = rootMap.find("cards");
    if (cardsIt != rootMap.end() && cardsIt->second.getType() == cocos2d::Value::Type::VECTOR) {
        for (const auto& cardVal : cardsIt->second.asValueVector()) {
            if (cardVal.getType() != cocos2d::Value::Type::MAP) {
                continue;
            }
            const auto& cardMap = cardVal.asValueMap();
            const int cardId = JsonUtils::getIntFromJson(cardMap, "cardId");
            const int faceVal = JsonUtils::getIntFromJson(cardMap, "face");
            const int suitVal = JsonUtils::getIntFromJson(cardMap, "suit");
            const float posX = JsonUtils::getFloatFromJson(cardMap, "posX");
            const float posY = JsonUtils::getFloatFromJson(cardMap, "posY");
            const int areaVal = JsonUtils::getIntFromJson(cardMap, "area");
            const bool isFlipped = JsonUtils::getBoolFromJson(cardMap, "isFlipped");
            const bool isUsed = JsonUtils::getBoolFromJson(cardMap, "isUsed");

            CardModel card(cardId,
                           static_cast<CardFaceType>(faceVal),
                           static_cast<CardSuitType>(suitVal),
                           cocos2d::Vec2(posX, posY),
                           static_cast<CardAreaType>(areaVal),
                           isFlipped,
                           isUsed);
            addCard(card);
        }
    }
}

} // namespace cardgame
