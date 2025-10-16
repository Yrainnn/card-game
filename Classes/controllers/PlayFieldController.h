#pragma once

#include "Classes/controllers/CardController.h"
#include "Classes/managers/UndoManager.h"
#include "Classes/models/GameModel.h"
#include "Classes/services/CardMatchService.h"
#include "Classes/views/PlayFieldView.h"
#include "Classes/views/TrayView.h"

namespace cardgame {

class PlayFieldController {
public:
    void init(GameModel* gameModel,
              PlayFieldView* playFieldView,
              TrayView* trayView,
              CardController* cardController,
              UndoManager* undoManager);

    bool handleCardClick(int cardId);
    void syncPlayFieldView();

private:
    GameModel* _gameModel = nullptr;
    PlayFieldView* _playFieldView = nullptr;
    TrayView* _trayView = nullptr;
    CardController* _cardController = nullptr;
    UndoManager* _undoManager = nullptr;
};

} // namespace cardgame
