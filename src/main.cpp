#include <iostream>
#include <string>

#include "Classes/controllers/GameController.h"
#include "Classes/models/CardEnums.h"
#include "Classes/models/CardModel.h"
#include "Classes/models/GameModel.h"
#include "cocos2d.h"

namespace {

std::string areaToString(cardgame::CardAreaType area) {
    switch (area) {
    case cardgame::CardAreaType::AREA_PLAYFIELD:
        return "Playfield";
    case cardgame::CardAreaType::AREA_STACK:
        return "Stack";
    case cardgame::CardAreaType::AREA_TRAY:
        return "Tray";
    case cardgame::CardAreaType::AREA_NONE:
    default:
        return "None";
    }
}

} // namespace

int main() {
    // Initialise the stub director with a predictable frame size so coordinate conversions work.
    auto* director = cocos2d::Director::getInstance();
    if (auto* view = director->getOpenGLView()) {
        view->setFrameSize(1080.0f, 1920.0f);
    }

    cardgame::GameController controller;
    controller.startGame(1);

    auto* model = controller.getGameModel();
    if (!model) {
        std::cerr << "Failed to initialise the game model." << std::endl;
        return 1;
    }

    std::cout << "Loaded level: " << model->getLevelId() << '\n';
    std::cout << "Tray card id: " << model->getCurrentTrayCardId() << '\n';
    std::cout << "Stack size: " << model->getStackCardCount() << '\n';

    std::cout << "\nActive cards:" << '\n';
    for (const auto& [cardId, card] : model->getAllCards()) {
        std::cout << "  Card " << cardId << " -> " << cardgame::toString(card.getSuitType()) << ' '
                  << cardgame::toString(card.getFaceType()) << ", area=" << areaToString(card.getCurrentArea())
                  << ", flipped=" << (card.isFlipped() ? "yes" : "no")
                  << ", used=" << (card.isUsed() ? "yes" : "no") << '\n';
    }

    const bool gameOver = controller.checkGameOver();
    std::cout << "\nGame over status: " << (gameOver ? "game finished" : "moves remaining") << '\n';

    return 0;
}

