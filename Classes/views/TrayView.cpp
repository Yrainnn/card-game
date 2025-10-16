#include "Classes/views/TrayView.h"

#include <new>

namespace cardgame {

TrayView* TrayView::create(const CardResConfig* resConfig) {
    TrayView* ret = new (std::nothrow) TrayView();
    if (ret && ret->init(resConfig)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool TrayView::init(const CardResConfig* resConfig) {
    if (!Node::init()) {
        return false;
    }

    _resConfig = resConfig;

    _trayLayer = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 0), 1080, 500);
    addChild(_trayLayer);

    return true;
}

void TrayView::setTrayCardView(int cardId, const CardModel& cardModel) {
    if (_currentTrayCardView) {
        _currentTrayCardView->removeFromParent();
        _currentTrayCardView = nullptr;
    }
    _currentTrayCardView = CardView::create(cardId, _resConfig);
    if (_currentTrayCardView) {
        _currentTrayCardView->initView(cardModel);
        _trayLayer->addChild(_currentTrayCardView);
    }
}

void TrayView::playTrayReplaceAnim(int newCardId, const CardModel& newCardModel) {
    if (_currentTrayCardView) {
        CardView* oldView = _currentTrayCardView;
        oldView->runAction(cocos2d::Sequence::create(
            cocos2d::FadeOut::create(0.2f),
            cocos2d::CallFunc::create([this, oldView]() {
                if (oldView) {
                    oldView->removeFromParent();
                }
                if (_currentTrayCardView == oldView) {
                    _currentTrayCardView = nullptr;
                }
            }),
            nullptr));
    }

    auto* incomingView = CardView::create(newCardId, _resConfig);
    if (incomingView) {
        incomingView->initView(newCardModel);
        incomingView->setOpacity(0);
        _trayLayer->addChild(incomingView);
        incomingView->runAction(cocos2d::FadeIn::create(0.2f));
        _currentTrayCardView = incomingView;
    }
}

} // namespace cardgame
