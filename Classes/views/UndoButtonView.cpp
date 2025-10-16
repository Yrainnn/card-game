#include "Classes/views/UndoButtonView.h"

#include <new>

namespace cardgame {

UndoButtonView* UndoButtonView::create() {
    UndoButtonView* ret = new (std::nothrow) UndoButtonView();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool UndoButtonView::init() {
    if (!Node::init()) {
        return false;
    }

    _undoBtn = cocos2d::MenuItemImage::create("res/ui/undo_normal.png",
                                              "res/ui/undo_pressed.png",
                                              "res/ui/undo_disabled.png",
                                              CC_CALLBACK_1(UndoButtonView::onUndoPressed, this));
    auto menu = cocos2d::Menu::create(_undoBtn, nullptr);
    addChild(menu);

    return true;
}

void UndoButtonView::setButtonEnabled(bool enabled) {
    if (_undoBtn) {
        _undoBtn->setEnabled(enabled);
    }
}

void UndoButtonView::setOnUndoCallback(const std::function<void()>& callback) {
    _onUndoCallback = callback;
}

void UndoButtonView::onUndoPressed(cocos2d::Ref* /*sender*/) {
    if (_onUndoCallback) {
        _onUndoCallback();
    }
}

} // namespace cardgame
