#pragma once

#include <functional>

#include "cocos2d.h"

#include "Classes/configs/models/CardResConfig.h"
#include "Classes/models/CardModel.h"
#include "Classes/views/CardView.h"

namespace cardgame {

class StackView : public cocos2d::Node {
public:
    static StackView* create(const CardResConfig* resConfig);

    void initTopCardView(int cardId, const CardModel& cardModel);
    void updateTopCardView(int cardId, const CardModel& cardModel);
    void hideStack();

    void setOnDrawCardCallback(const std::function<void()>& callback);

private:
    bool init(const CardResConfig* resConfig);

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    cocos2d::LayerColor* _stackLayer = nullptr;
    CardView* _currentTopCardView = nullptr;
    const CardResConfig* _resConfig = nullptr;
    std::function<void()> _onDrawCardCallback;
};

} // namespace cardgame
