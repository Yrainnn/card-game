#pragma once

#include "Classes/managers/UndoManager.h"
#include "Classes/models/GameModel.h"
#include "Classes/views/PlayFieldView.h"
#include "Classes/views/StackView.h"
#include "Classes/views/TrayView.h"
#include "Classes/views/UndoButtonView.h"

namespace cardgame {

class UndoController {
public:
    void init(UndoManager* undoManager,
              UndoButtonView* buttonView,
              GameModel* gameModel,
              PlayFieldView* playFieldView,
              StackView* stackView,
              TrayView* trayView);

    void handleUndo();
    void syncUndoButtonState();

private:
    void restoreViewsFromModel();

    UndoManager* _undoManager = nullptr;
    UndoButtonView* _undoBtnView = nullptr;
    GameModel* _gameModel = nullptr;
    PlayFieldView* _playFieldView = nullptr;
    StackView* _stackView = nullptr;
    TrayView* _trayView = nullptr;
};

} // namespace cardgame
