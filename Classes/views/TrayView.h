#pragma once

#include "cocos2d.h"

#include "Classes/configs/models/CardResConfig.h"
#include "Classes/models/CardModel.h"
#include "Classes/views/CardView.h"

namespace cardgame {

class TrayView : public cocos2d::Node {
public:
    static TrayView* create(const CardResConfig* resConfig);

    void setTrayCardView(int cardId, const CardModel& cardModel);
    void playTrayReplaceAnim(int newCardId, const CardModel& newCardModel);

private:
    bool init(const CardResConfig* resConfig);

    cocos2d::LayerColor* _trayLayer = nullptr;
    CardView* _currentTrayCardView = nullptr;
    const CardResConfig* _resConfig = nullptr;
};

} // namespace cardgame
