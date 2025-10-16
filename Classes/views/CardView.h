#pragma once

#include <functional>

#include "cocos2d.h"

#include "Classes/configs/models/CardResConfig.h"
#include "Classes/models/CardModel.h"

namespace cardgame {

class CardView : public cocos2d::Node {
public:
    static CardView* create(int cardId, const CardResConfig* resConfig);

    void initView(const CardModel& cardModel);
    void updatePos(const cocos2d::Vec2& pos);
    void flipCard(bool isFlipped);
    void playMoveAnim(const cocos2d::Vec2& targetPos, float duration = 0.3f);

    void setOnClickCallback(const std::function<void(int)>& callback);

    int getCardId() const { return _cardId; }

private:
    bool init(int cardId, const CardResConfig* resConfig);

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    void updateTexture(bool isFlipped);

    cocos2d::Sprite* _cardSprite = nullptr;
    int _cardId = -1;
    std::function<void(int)> _onClickCallback;
    const CardResConfig* _resConfig = nullptr;
    bool _isFlipped = false;
    CardFaceType _faceType = CardFaceType::CFT_ACE;
    CardSuitType _suitType = CardSuitType::CST_CLUBS;
};

} // namespace cardgame
