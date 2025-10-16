#pragma once

#include <functional>
#include <unordered_map>

#include "cocos2d.h"

#include "Classes/configs/models/CardResConfig.h"
#include "Classes/models/CardModel.h"
#include "Classes/views/CardView.h"

namespace cardgame {

class PlayFieldView : public cocos2d::Node {
public:
    static PlayFieldView* create(const CardResConfig* resConfig);

    void addCardView(int cardId, const CardModel& cardModel);
    void updateCardView(int cardId, const CardModel& cardModel);
    void removeCardView(int cardId);

    void playCardMatchAnim(int cardId, const cocos2d::Vec2& targetPos);

    void setCardClickCallback(const std::function<void(int)>& callback);

private:
    bool init(const CardResConfig* resConfig);

    cocos2d::LayerColor* _playfieldLayer = nullptr;
    std::unordered_map<int, CardView*> _cardViews;
    const CardResConfig* _resConfig = nullptr;
    std::function<void(int)> _cardClickCallback;
};

} // namespace cardgame
