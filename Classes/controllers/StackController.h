#pragma once

#include "Classes/managers/UndoManager.h"
#include "Classes/models/GameModel.h"
#include "Classes/views/StackView.h"
#include "Classes/views/TrayView.h"

namespace cardgame {

class StackController {
public:
    void init(GameModel* gameModel, StackView* stackView, TrayView* trayView, UndoManager* undoManager);

    bool handleDrawCard();
    void syncStackView();

private:
    GameModel* _gameModel = nullptr;
    StackView* _stackView = nullptr;
    TrayView* _trayView = nullptr;
    UndoManager* _undoManager = nullptr;
};

} // namespace cardgame
