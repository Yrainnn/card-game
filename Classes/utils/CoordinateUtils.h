#pragma once

#include "cocos2d.h"

namespace cardgame {

class CoordinateUtils {
public:
    static cocos2d::Vec2 designToScreen(const cocos2d::Vec2& designPos);
};

} // namespace cardgame
