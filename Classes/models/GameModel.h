#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Classes/models/CardModel.h"

namespace cardgame {

class GameModel {
public:
    GameModel() = default;

    CardModel& getCardById(int cardId);
    const CardModel& getCardById(int cardId) const;

    void addCard(CardModel cardModel);
    const std::unordered_map<int, CardModel>& getAllCards() const;

    int getCurrentTrayCardId() const;
    void setCurrentTrayCardId(int cardId);

    int getLevelId() const;
    void setLevelId(int levelId);

    bool isGameOver() const;
    void setGameOver(bool gameOver);

    const std::vector<int>& getStackCardIds() const;
    void setStackCardIds(const std::vector<int>& ids);
    void removeStackTopCard();
    void pushStackCard(int cardId);

    size_t getStackCardCount() const;

    void markCardUsed(int cardId);

    std::string toJson() const;
    void fromJson(const std::string& jsonStr);

private:
    std::unordered_map<int, CardModel> _allCards;
    int _currentTrayCardId = -1;
    std::vector<int> _stackCardIds;
    int _levelId = 0;
    bool _isGameOver = false;
};

} // namespace cardgame
