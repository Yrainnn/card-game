#include "Classes/controllers/GameController.h"

#include "Classes/services/CardMatchService.h"

namespace cardgame {

GameController::GameController()
    : _gameModel(std::make_unique<GameModel>())
    , _cardController(std::make_unique<CardController>())
    , _playFieldController(std::make_unique<PlayFieldController>())
    , _stackController(std::make_unique<StackController>())
    , _undoController(std::make_unique<UndoController>())
    , _undoManager(std::make_unique<UndoManager>())
    , _cardResConfig(std::make_unique<CardResConfig>()) {}

GameController::~GameController() {
    if (_gameView) {
        _gameView->release();
        _gameView = nullptr;
    }
}

void GameController::startGame(int levelId) {
    LevelConfig levelConfig = LevelConfigLoader::loadLevelConfig(levelId);
    GameModel generatedModel = GameModelGeneratorService::generateGameModel(levelConfig);
    *_gameModel = std::move(generatedModel);

    _undoManager->init(_gameModel.get());

    if (!_gameView) {
        _gameView = GameView::create(_cardResConfig.get());
        _gameView->retain();
    }

    _cardController->init(_gameModel.get(), _gameView->getPlayFieldView(), _gameView->getTrayView());
    _playFieldController->init(_gameModel.get(),
                               _gameView->getPlayFieldView(),
                               _gameView->getTrayView(),
                               _cardController.get(),
                               _undoManager.get());
    _stackController->init(
        _gameModel.get(), _gameView->getStackView(), _gameView->getTrayView(), _undoManager.get());
    _undoController->init(_undoManager.get(),
                          _gameView->getUndoButtonView(),
                          _gameModel.get(),
                          _gameView->getPlayFieldView(),
                          _gameView->getStackView(),
                          _gameView->getTrayView());

    bindViewEvents();

    _playFieldController->syncPlayFieldView();
    _stackController->syncStackView();
    if (auto* trayView = _gameView->getTrayView()) {
        const int trayId = _gameModel->getCurrentTrayCardId();
        if (trayId >= 0) {
            trayView->setTrayCardView(trayId, _gameModel->getCardById(trayId));
        }
    }
    _undoController->syncUndoButtonState();

    if (_gameView) {
        _gameView->playGameEnterAnim();
    }
}

void GameController::bindViewEvents() {
    if (!_gameView) {
        return;
    }

    if (auto* playfield = _gameView->getPlayFieldView()) {
        playfield->setCardClickCallback([this](int cardId) {
            if (_playFieldController->handleCardClick(cardId)) {
                _undoController->syncUndoButtonState();
            }
        });
    }

    if (auto* stackView = _gameView->getStackView()) {
        stackView->setOnDrawCardCallback([this]() {
            if (_stackController->handleDrawCard()) {
                _undoController->syncUndoButtonState();
            }
        });
    }

    if (auto* undoBtn = _gameView->getUndoButtonView()) {
        undoBtn->setOnUndoCallback([this]() { _undoController->handleUndo(); });
    }
}

bool GameController::checkGameOver() {
    if (!_gameModel) {
        return false;
    }

    if (_gameModel->getStackCardCount() > 0) {
        return false;
    }

    const int trayId = _gameModel->getCurrentTrayCardId();
    if (trayId < 0) {
        _gameModel->setGameOver(true);
        return true;
    }

    const CardModel& trayCard = _gameModel->getCardById(trayId);
    for (const auto& entry : _gameModel->getAllCards()) {
        const CardModel& card = entry.second;
        if (card.getCurrentArea() == CardAreaType::AREA_PLAYFIELD && !card.isUsed()) {
            if (CardMatchService::isMatchable(trayCard.getFaceType(), card.getFaceType())) {
                return false;
            }
        }
    }

    _gameModel->setGameOver(true);
    return true;
}

} // namespace cardgame
