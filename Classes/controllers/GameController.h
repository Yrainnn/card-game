#pragma once

#include <memory>

#include "Classes/configs/loaders/LevelConfigLoader.h"
#include "Classes/configs/models/CardResConfig.h"
#include "Classes/controllers/CardController.h"
#include "Classes/controllers/PlayFieldController.h"
#include "Classes/controllers/StackController.h"
#include "Classes/controllers/UndoController.h"
#include "Classes/managers/UndoManager.h"
#include "Classes/models/GameModel.h"
#include "Classes/services/GameModelGeneratorService.h"
#include "Classes/views/GameView.h"

namespace cardgame {

class GameController {
public:
    GameController();
    ~GameController();

    void startGame(int levelId);
    void bindViewEvents();
    bool checkGameOver();

    GameView* getGameView() const { return _gameView; }

private:
    std::unique_ptr<GameModel> _gameModel;
    GameView* _gameView = nullptr;
    std::unique_ptr<CardController> _cardController;
    std::unique_ptr<PlayFieldController> _playFieldController;
    std::unique_ptr<StackController> _stackController;
    std::unique_ptr<UndoController> _undoController;
    std::unique_ptr<UndoManager> _undoManager;
    std::unique_ptr<CardResConfig> _cardResConfig;
};

} // namespace cardgame
