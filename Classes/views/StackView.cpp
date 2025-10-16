#include "Classes/views/StackView.h"

#include <new>

namespace cardgame {

StackView* StackView::create(const CardResConfig* resConfig) {
    StackView* ret = new (std::nothrow) StackView();
    if (ret && ret->init(resConfig)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool StackView::init(const CardResConfig* resConfig) {
    if (!Node::init()) {
        return false;
    }

    _resConfig = resConfig;

    _stackLayer = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 0), 1080, 580);
    addChild(_stackLayer);

    auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(StackView::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(StackView::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

void StackView::initTopCardView(int cardId, const CardModel& cardModel) {
    if (_currentTopCardView) {
        _currentTopCardView->removeFromParent();
        _currentTopCardView = nullptr;
    }
    _currentTopCardView = CardView::create(cardId, _resConfig);
    if (_currentTopCardView) {
        _currentTopCardView->initView(cardModel);
        _stackLayer->addChild(_currentTopCardView);
    }
    if (_stackLayer) {
        _stackLayer->setVisible(true);
    }
}

void StackView::updateTopCardView(int cardId, const CardModel& cardModel) {
    if (!_currentTopCardView) {
        initTopCardView(cardId, cardModel);
        return;
    }
    _currentTopCardView->initView(cardModel);
    if (_stackLayer) {
        _stackLayer->setVisible(true);
    }
}

void StackView::hideStack() {
    if (_stackLayer) {
        _stackLayer->setVisible(false);
    }
    if (_currentTopCardView) {
        _currentTopCardView->removeFromParent();
        _currentTopCardView = nullptr;
    }
}

void StackView::setOnDrawCardCallback(const std::function<void()>& callback) {
    _onDrawCardCallback = callback;
}

bool StackView::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* /*event*/) {
    const cocos2d::Vec2 local = convertToNodeSpace(touch->getLocation());
    const cocos2d::Rect bounds = cocos2d::Rect(0, 0, 1080, 580);
    return bounds.containsPoint(local);
}

void StackView::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* /*event*/) {
    const cocos2d::Vec2 local = convertToNodeSpace(touch->getLocation());
    const cocos2d::Rect bounds = cocos2d::Rect(0, 0, 1080, 580);
    if (bounds.containsPoint(local) && _onDrawCardCallback) {
        _onDrawCardCallback();
    }
}

} // namespace cardgame
