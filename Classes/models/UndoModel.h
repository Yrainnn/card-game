#pragma once

#include <unordered_map>
#include <vector>

#include "Classes/models/CardModel.h"

namespace cardgame {

enum class UndoOperationType {
    OP_NONE = 0,
    OP_TRAY_REPLACE,
    OP_CARD_MATCH,
    OP_COMBINED
};

struct CardStateSnapshot {
    cocos2d::Vec2 pos;
    CardAreaType area = CardAreaType::AREA_NONE;
    bool isFlipped = false;
    bool isUsed = false;
};

class UndoModel {
public:
    UndoModel() = default;
    UndoModel(UndoOperationType opType,
              int preTrayId,
              std::unordered_map<int, CardStateSnapshot> cardSnapshots,
              std::vector<int> stackCardIdsBefore = {});

    UndoOperationType getOperationType() const;
    const std::unordered_map<int, CardStateSnapshot>& getCardSnapshots() const;
    int getPreTrayCardId() const;
    const std::vector<int>& getStackCardIdsBefore() const;

private:
    UndoOperationType _operationType = UndoOperationType::OP_NONE;
    std::unordered_map<int, CardStateSnapshot> _cardSnapshots;
    int _preTrayCardId = -1;
    std::vector<int> _stackCardIdsBefore;
};

} // namespace cardgame
