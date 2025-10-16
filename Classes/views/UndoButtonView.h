#pragma once

#include <functional>

#include "cocos2d.h"

namespace cardgame {

class UndoButtonView : public cocos2d::Node {
public:
    static UndoButtonView* create();

    void setButtonEnabled(bool enabled);
    void setOnUndoCallback(const std::function<void()>& callback);

private:
    bool init();

    void onUndoPressed(cocos2d::Ref* sender);

    cocos2d::MenuItemImage* _undoBtn = nullptr;
    std::function<void()> _onUndoCallback;
};

} // namespace cardgame
