#pragma once

#include "Classes/models/GameModel.h"
#include "Classes/models/UndoModel.h"
#include "Classes/views/PlayFieldView.h"
#include "Classes/views/TrayView.h"

namespace cardgame {

class CardController {
public:
    void init(GameModel* gameModel, PlayFieldView* playFieldView, TrayView* trayView);

    bool isCardOperable(int cardId) const;
    void updateCardState(int cardId, const CardStateSnapshot& snapshot);
    void flipCard(int cardId, bool isFlipped);

private:
    GameModel* _gameModel = nullptr;
    PlayFieldView* _playFieldView = nullptr;
    TrayView* _trayView = nullptr;
};

} // namespace cardgame
