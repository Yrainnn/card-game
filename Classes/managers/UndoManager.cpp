#include "Classes/managers/UndoManager.h"

#include <unordered_map>
#include <utility>

namespace cardgame {

void UndoManager::init(GameModel* gameModel) {
    _gameModel = gameModel;
    _undoRecords.clear();
}

void UndoManager::recordMatchOperation(int matchedCardId) {
    if (!_gameModel) {
        return;
    }

    std::unordered_map<int, CardStateSnapshot> snapshots;
    const int trayId = _gameModel->getCurrentTrayCardId();
    snapshots.emplace(trayId, buildSnapshot(_gameModel->getCardById(trayId)));
    snapshots.emplace(matchedCardId, buildSnapshot(_gameModel->getCardById(matchedCardId)));

    _undoRecords.emplace_back(
        UndoOperationType::OP_CARD_MATCH, trayId, std::move(snapshots), _gameModel->getStackCardIds());
}

void UndoManager::recordDrawOperation() {
    if (!_gameModel || _gameModel->getStackCardCount() == 0) {
        return;
    }

    std::unordered_map<int, CardStateSnapshot> snapshots;
    const int trayId = _gameModel->getCurrentTrayCardId();
    snapshots.emplace(trayId, buildSnapshot(_gameModel->getCardById(trayId)));

    const int topStackId = _gameModel->getStackCardIds().front();
    snapshots.emplace(topStackId, buildSnapshot(_gameModel->getCardById(topStackId)));

    _undoRecords.emplace_back(UndoOperationType::OP_TRAY_REPLACE,
                              trayId,
                              std::move(snapshots),
                              _gameModel->getStackCardIds());
}

bool UndoManager::hasUndoRecord() const { return !_undoRecords.empty(); }

UndoModel UndoManager::getLastUndoModel() const {
    if (_undoRecords.empty()) {
        return UndoModel();
    }
    return _undoRecords.back();
}

void UndoManager::rollbackToSnapshot(const UndoModel& undoModel) {
    if (!_gameModel) {
        return;
    }

    _gameModel->setCurrentTrayCardId(undoModel.getPreTrayCardId());
    if (!undoModel.getStackCardIdsBefore().empty()) {
        _gameModel->setStackCardIds(undoModel.getStackCardIdsBefore());
    }

    for (const auto& entry : undoModel.getCardSnapshots()) {
        CardModel& card = _gameModel->getCardById(entry.first);
        const CardStateSnapshot& snapshot = entry.second;
        card.setCurrentPos(snapshot.pos);
        card.setCurrentArea(snapshot.area);
        card.setIsFlipped(snapshot.isFlipped);
        card.setIsUsed(snapshot.isUsed);
    }
}

void UndoManager::removeLastUndoRecord() {
    if (!_undoRecords.empty()) {
        _undoRecords.pop_back();
    }
}

CardStateSnapshot UndoManager::buildSnapshot(const CardModel& cardModel) const {
    CardStateSnapshot snapshot;
    snapshot.pos = cardModel.getCurrentPos();
    snapshot.area = cardModel.getCurrentArea();
    snapshot.isFlipped = cardModel.isFlipped();
    snapshot.isUsed = cardModel.isUsed();
    return snapshot;
}

} // namespace cardgame
