#include "Classes/utils/CoordinateUtils.h"

namespace cardgame {

cocos2d::Vec2 CoordinateUtils::designToScreen(const cocos2d::Vec2& designPos) {
    auto* director = cocos2d::Director::getInstance();
    const cocos2d::Size designSize(1080.0f, 2080.0f);
    const cocos2d::Size frameSize = director->getOpenGLView()->getFrameSize();

    float scaleX = frameSize.width / designSize.width;
    float scaleY = frameSize.height / designSize.height;

    return cocos2d::Vec2(designPos.x * scaleX, designPos.y * scaleY);
}

} // namespace cardgame
