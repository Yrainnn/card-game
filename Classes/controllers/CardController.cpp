#include "Classes/controllers/CardController.h"

namespace cardgame {

void CardController::init(GameModel* gameModel, PlayFieldView* playFieldView, TrayView* trayView) {
    _gameModel = gameModel;
    _playFieldView = playFieldView;
    _trayView = trayView;
}

bool CardController::isCardOperable(int cardId) const {
    if (!_gameModel) {
        return false;
    }
    const CardModel& card = _gameModel->getCardById(cardId);
    if (card.isUsed()) {
        return false;
    }
    if (!card.isFlipped()) {
        return false;
    }
    CardAreaType area = card.getCurrentArea();
    return area == CardAreaType::AREA_PLAYFIELD || area == CardAreaType::AREA_TRAY;
}

void CardController::updateCardState(int cardId, const CardStateSnapshot& snapshot) {
    if (!_gameModel) {
        return;
    }
    CardModel& card = _gameModel->getCardById(cardId);
    card.setCurrentPos(snapshot.pos);
    card.setCurrentArea(snapshot.area);
    card.setIsFlipped(snapshot.isFlipped);
    card.setIsUsed(snapshot.isUsed);

    if (snapshot.area == CardAreaType::AREA_PLAYFIELD && _playFieldView) {
        _playFieldView->updateCardView(cardId, card);
    } else if (snapshot.area == CardAreaType::AREA_TRAY && _trayView) {
        _trayView->setTrayCardView(cardId, card);
    }
}

void CardController::flipCard(int cardId, bool isFlipped) {
    if (!_gameModel) {
        return;
    }
    CardModel& card = _gameModel->getCardById(cardId);
    card.setIsFlipped(isFlipped);

    if (card.getCurrentArea() == CardAreaType::AREA_PLAYFIELD && _playFieldView) {
        _playFieldView->updateCardView(cardId, card);
    } else if (card.getCurrentArea() == CardAreaType::AREA_TRAY && _trayView) {
        _trayView->setTrayCardView(cardId, card);
    }
}

} // namespace cardgame
