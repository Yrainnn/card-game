#include "Classes/views/GameView.h"

#include <new>

namespace cardgame {

GameView* GameView::create(const CardResConfig* resConfig) {
    GameView* ret = new (std::nothrow) GameView();
    if (ret && ret->init(resConfig)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool GameView::init(const CardResConfig* resConfig) {
    if (!Node::init()) {
        return false;
    }

    _resConfig = resConfig;

    _playFieldView = PlayFieldView::create(resConfig);
    _stackView = StackView::create(resConfig);
    _trayView = TrayView::create(resConfig);
    _undoBtnView = UndoButtonView::create();

    if (_playFieldView) {
        _playFieldView->setPosition(0, 580);
        addChild(_playFieldView);
    }
    if (_stackView) {
        _stackView->setPosition(0, 0);
        addChild(_stackView);
    }
    if (_trayView) {
        _trayView->setPosition(0, 1500);
        addChild(_trayView);
    }
    if (_undoBtnView) {
        _undoBtnView->setPosition(900, 1900);
        addChild(_undoBtnView);
    }

    return true;
}

void GameView::playGameEnterAnim() {
    if (_playFieldView) {
        _playFieldView->setOpacity(0);
        _playFieldView->runAction(cocos2d::FadeIn::create(0.4f));
    }
    if (_stackView) {
        _stackView->setOpacity(0);
        _stackView->runAction(cocos2d::FadeIn::create(0.4f));
    }
    if (_trayView) {
        _trayView->setOpacity(0);
        _trayView->runAction(cocos2d::FadeIn::create(0.4f));
    }
}

} // namespace cardgame
