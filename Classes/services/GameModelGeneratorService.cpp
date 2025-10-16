#include "Classes/services/GameModelGeneratorService.h"

#include <vector>

namespace cardgame {

GameModel GameModelGeneratorService::generateGameModel(const LevelConfig& levelConfig,
                                                       int initTrayCardId) {
    GameModel gameModel;
    gameModel.setLevelId(levelConfig.getLevelId());

    int nextCardId = 0;
    for (const auto& staticCard : levelConfig.getPlayfieldCards()) {
        CardModel card(nextCardId++,
                       staticCard.faceType,
                       staticCard.suitType,
                       staticCard.initPos,
                       CardAreaType::AREA_PLAYFIELD,
                       true,
                       false);
        gameModel.addCard(card);
    }

    std::vector<int> stackIds;
    for (const auto& staticCard : levelConfig.getStackCards()) {
        CardModel card(nextCardId,
                       staticCard.faceType,
                       staticCard.suitType,
                       cocos2d::Vec2::ZERO,
                       CardAreaType::AREA_STACK,
                       false,
                       false);
        stackIds.push_back(nextCardId);
        gameModel.addCard(card);
        ++nextCardId;
    }

    int trayCardId = initTrayCardId;
    if (trayCardId < 0) {
        if (!stackIds.empty()) {
            trayCardId = stackIds.front();
            stackIds.erase(stackIds.begin());
        } else if (!levelConfig.getPlayfieldCards().empty()) {
            trayCardId = 0;
        }
    }

    if (trayCardId >= 0) {
        CardModel& trayCard = gameModel.getCardById(trayCardId);
        trayCard.setCurrentArea(CardAreaType::AREA_TRAY);
        trayCard.setIsFlipped(true);
        trayCard.setIsUsed(false);
    }

    gameModel.setCurrentTrayCardId(trayCardId);
    gameModel.setStackCardIds(stackIds);

    return gameModel;
}

} // namespace cardgame
