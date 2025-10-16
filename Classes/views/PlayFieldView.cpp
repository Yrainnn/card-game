#include "Classes/views/PlayFieldView.h"

#include <new>

namespace cardgame {

PlayFieldView* PlayFieldView::create(const CardResConfig* resConfig) {
    PlayFieldView* ret = new (std::nothrow) PlayFieldView();
    if (ret && ret->init(resConfig)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool PlayFieldView::init(const CardResConfig* resConfig) {
    if (!Node::init()) {
        return false;
    }

    _resConfig = resConfig;

    _playfieldLayer = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 0), 1080, 1500);
    addChild(_playfieldLayer);

    return true;
}

void PlayFieldView::addCardView(int cardId, const CardModel& cardModel) {
    auto* cardView = CardView::create(cardId, _resConfig);
    if (!cardView) {
        return;
    }
    cardView->initView(cardModel);
    if (_cardClickCallback) {
        cardView->setOnClickCallback(_cardClickCallback);
    }
    _playfieldLayer->addChild(cardView);
    _cardViews[cardId] = cardView;
}

void PlayFieldView::updateCardView(int cardId, const CardModel& cardModel) {
    auto iter = _cardViews.find(cardId);
    if (iter == _cardViews.end()) {
        addCardView(cardId, cardModel);
        return;
    }
    iter->second->initView(cardModel);
}

void PlayFieldView::removeCardView(int cardId) {
    auto iter = _cardViews.find(cardId);
    if (iter == _cardViews.end()) {
        return;
    }
    iter->second->removeFromParent();
    _cardViews.erase(iter);
}

void PlayFieldView::playCardMatchAnim(int cardId, const cocos2d::Vec2& targetPos) {
    auto iter = _cardViews.find(cardId);
    if (iter == _cardViews.end()) {
        return;
    }
    iter->second->playMoveAnim(targetPos);
}

void PlayFieldView::setCardClickCallback(const std::function<void(int)>& callback) {
    _cardClickCallback = callback;
    for (auto& entry : _cardViews) {
        entry.second->setOnClickCallback(callback);
    }
}

} // namespace cardgame
