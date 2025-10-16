#include "Classes/views/CardView.h"

#include <new>

namespace cardgame {

CardView* CardView::create(int cardId, const CardResConfig* resConfig) {
    CardView* ret = new (std::nothrow) CardView();
    if (ret && ret->init(cardId, resConfig)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CardView::init(int cardId, const CardResConfig* resConfig) {
    if (!Node::init()) {
        return false;
    }

    _cardId = cardId;
    _resConfig = resConfig;

    _cardSprite = cocos2d::Sprite::create();
    if (_cardSprite) {
        addChild(_cardSprite);
    }

    auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(CardView::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(CardView::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

void CardView::initView(const CardModel& cardModel) {
    _cardId = cardModel.getCardId();
    _faceType = cardModel.getFaceType();
    _suitType = cardModel.getSuitType();
    updatePos(cardModel.getCurrentPos());
    flipCard(cardModel.isFlipped());
}

void CardView::updatePos(const cocos2d::Vec2& pos) { setPosition(pos); }

void CardView::flipCard(bool isFlipped) {
    _isFlipped = isFlipped;
    updateTexture(isFlipped);
}

void CardView::playMoveAnim(const cocos2d::Vec2& targetPos, float duration) {
    auto moveAction = cocos2d::MoveTo::create(duration, targetPos);
    runAction(moveAction);
}

void CardView::setOnClickCallback(const std::function<void(int)>& callback) {
    _onClickCallback = callback;
}

bool CardView::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* /*event*/) {
    const cocos2d::Vec2 localPoint = convertToNodeSpace(touch->getLocation());
    cocos2d::Rect bounds = _cardSprite ? _cardSprite->getBoundingBox() : cocos2d::Rect();
    if (bounds.containsPoint(localPoint)) {
        return true;
    }
    return false;
}

void CardView::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* /*event*/) {
    const cocos2d::Vec2 localPoint = convertToNodeSpace(touch->getLocation());
    cocos2d::Rect bounds = _cardSprite ? _cardSprite->getBoundingBox() : cocos2d::Rect();
    if (bounds.containsPoint(localPoint) && _onClickCallback) {
        _onClickCallback(_cardId);
    }
}

void CardView::updateTexture(bool isFlipped) {
    if (!_cardSprite || !_resConfig) {
        return;
    }
    std::string resPath =
        isFlipped ? _resConfig->getCardResPath(_suitType, _faceType) : "res/cards/back.png";
    _cardSprite->setTexture(resPath);
}

} // namespace cardgame
