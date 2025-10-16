#include "Classes/controllers/PlayFieldController.h"

namespace cardgame {

void PlayFieldController::init(GameModel* gameModel,
                               PlayFieldView* playFieldView,
                               TrayView* trayView,
                               CardController* cardController,
                               UndoManager* undoManager) {
    _gameModel = gameModel;
    _playFieldView = playFieldView;
    _trayView = trayView;
    _cardController = cardController;
    _undoManager = undoManager;
}

bool PlayFieldController::handleCardClick(int cardId) {
    if (!_cardController || !_gameModel) {
        return false;
    }
    if (!_cardController->isCardOperable(cardId)) {
        return false;
    }

    const int trayCardId = _gameModel->getCurrentTrayCardId();
    CardModel& trayCard = _gameModel->getCardById(trayCardId);
    CardModel& clickedCard = _gameModel->getCardById(cardId);

    if (!CardMatchService::isMatchable(trayCard.getFaceType(), clickedCard.getFaceType())) {
        return false;
    }

    if (_undoManager) {
        _undoManager->recordMatchOperation(cardId);
    }

    clickedCard.setIsUsed(true);
    clickedCard.setCurrentArea(CardAreaType::AREA_TRAY);
    clickedCard.setIsFlipped(true);
    _gameModel->setCurrentTrayCardId(cardId);

    if (_playFieldView) {
        _playFieldView->playCardMatchAnim(cardId, trayCard.getCurrentPos());
        _playFieldView->removeCardView(cardId);
    }

    if (_trayView) {
        _trayView->setTrayCardView(cardId, clickedCard);
    }

    return true;
}

void PlayFieldController::syncPlayFieldView() {
    if (!_playFieldView || !_gameModel) {
        return;
    }

    for (const auto& entry : _gameModel->getAllCards()) {
        const CardModel& card = entry.second;
        if (card.getCurrentArea() == CardAreaType::AREA_PLAYFIELD && !card.isUsed()) {
            _playFieldView->updateCardView(card.getCardId(), card);
        }
    }
}

} // namespace cardgame
