#include "Classes/controllers/UndoController.h"

namespace cardgame {

void UndoController::init(UndoManager* undoManager,
                          UndoButtonView* buttonView,
                          GameModel* gameModel,
                          PlayFieldView* playFieldView,
                          StackView* stackView,
                          TrayView* trayView) {
    _undoManager = undoManager;
    _undoBtnView = buttonView;
    _gameModel = gameModel;
    _playFieldView = playFieldView;
    _stackView = stackView;
    _trayView = trayView;
}

void UndoController::handleUndo() {
    if (!_undoManager || !_undoManager->hasUndoRecord()) {
        return;
    }

    UndoModel undoModel = _undoManager->getLastUndoModel();
    _undoManager->rollbackToSnapshot(undoModel);
    _undoManager->removeLastUndoRecord();

    restoreViewsFromModel();
    syncUndoButtonState();
}

void UndoController::syncUndoButtonState() {
    if (_undoBtnView && _undoManager) {
        _undoBtnView->setButtonEnabled(_undoManager->hasUndoRecord());
    }
}

void UndoController::restoreViewsFromModel() {
    if (!_gameModel) {
        return;
    }

    if (_trayView) {
        const int trayId = _gameModel->getCurrentTrayCardId();
        if (trayId >= 0) {
            _trayView->setTrayCardView(trayId, _gameModel->getCardById(trayId));
        }
    }

    if (_stackView) {
        if (_gameModel->getStackCardCount() > 0) {
            const int topId = _gameModel->getStackCardIds().front();
            _stackView->updateTopCardView(topId, _gameModel->getCardById(topId));
        } else {
            _stackView->hideStack();
        }
    }

    if (_playFieldView) {
        for (const auto& entry : _gameModel->getAllCards()) {
            const CardModel& card = entry.second;
            if (card.getCurrentArea() == CardAreaType::AREA_PLAYFIELD && !card.isUsed()) {
                _playFieldView->updateCardView(card.getCardId(), card);
            } else {
                _playFieldView->removeCardView(card.getCardId());
            }
        }
    }
}

} // namespace cardgame
