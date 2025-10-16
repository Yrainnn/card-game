#include "Classes/controllers/StackController.h"

namespace cardgame {

void StackController::init(GameModel* gameModel,
                           StackView* stackView,
                           TrayView* trayView,
                           UndoManager* undoManager) {
    _gameModel = gameModel;
    _stackView = stackView;
    _trayView = trayView;
    _undoManager = undoManager;
}

bool StackController::handleDrawCard() {
    if (!_gameModel || _gameModel->getStackCardCount() == 0) {
        return false;
    }

    if (_undoManager) {
        _undoManager->recordDrawOperation();
    }

    const int newTrayCardId = _gameModel->getStackCardIds().front();
    _gameModel->removeStackTopCard();

    CardModel& trayCard = _gameModel->getCardById(newTrayCardId);
    trayCard.setCurrentArea(CardAreaType::AREA_TRAY);
    trayCard.setIsFlipped(true);
    trayCard.setIsUsed(false);

    const int previousTrayId = _gameModel->getCurrentTrayCardId();
    if (previousTrayId >= 0) {
        CardModel& previousTray = _gameModel->getCardById(previousTrayId);
        previousTray.setIsUsed(true);
    }

    _gameModel->setCurrentTrayCardId(newTrayCardId);

    if (_trayView) {
        _trayView->playTrayReplaceAnim(newTrayCardId, trayCard);
    }

    if (_stackView) {
        if (_gameModel->getStackCardCount() > 0) {
            const int nextTopId = _gameModel->getStackCardIds().front();
            _stackView->updateTopCardView(nextTopId, _gameModel->getCardById(nextTopId));
        } else {
            _stackView->hideStack();
        }
    }

    return true;
}

void StackController::syncStackView() {
    if (!_stackView || !_gameModel) {
        return;
    }

    if (_gameModel->getStackCardCount() == 0) {
        _stackView->hideStack();
    } else {
        const int topId = _gameModel->getStackCardIds().front();
        _stackView->updateTopCardView(topId, _gameModel->getCardById(topId));
    }
}

} // namespace cardgame
