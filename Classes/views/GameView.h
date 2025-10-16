#pragma once

#include "cocos2d.h"

#include "Classes/configs/models/CardResConfig.h"
#include "Classes/views/PlayFieldView.h"
#include "Classes/views/StackView.h"
#include "Classes/views/TrayView.h"
#include "Classes/views/UndoButtonView.h"

namespace cardgame {

class GameView : public cocos2d::Node {
public:
    static GameView* create(const CardResConfig* resConfig);

    PlayFieldView* getPlayFieldView() const { return _playFieldView; }
    StackView* getStackView() const { return _stackView; }
    TrayView* getTrayView() const { return _trayView; }
    UndoButtonView* getUndoButtonView() const { return _undoBtnView; }

    void playGameEnterAnim();

private:
    bool init(const CardResConfig* resConfig);

    PlayFieldView* _playFieldView = nullptr;
    StackView* _stackView = nullptr;
    TrayView* _trayView = nullptr;
    UndoButtonView* _undoBtnView = nullptr;
    const CardResConfig* _resConfig = nullptr;
};

} // namespace cardgame
