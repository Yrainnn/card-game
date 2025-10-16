#pragma once

#include <vector>

#include "Classes/models/GameModel.h"
#include "Classes/models/UndoModel.h"

namespace cardgame {

class UndoManager {
public:
    void init(GameModel* gameModel);

    void recordMatchOperation(int matchedCardId);
    void recordDrawOperation();

    bool hasUndoRecord() const;
    UndoModel getLastUndoModel() const;
    void rollbackToSnapshot(const UndoModel& undoModel);
    void removeLastUndoRecord();

private:
    CardStateSnapshot buildSnapshot(const CardModel& cardModel) const;

    std::vector<UndoModel> _undoRecords;
    GameModel* _gameModel = nullptr;
};

} // namespace cardgame
