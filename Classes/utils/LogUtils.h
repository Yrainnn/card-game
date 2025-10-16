#pragma once

#include <string>

#include "cocos2d.h"

namespace cardgame {

class LogUtils {
public:
    static void logDebug(const std::string& tag, const std::string& msg);
    static void logError(const std::string& tag, const std::string& msg);
};

} // namespace cardgame
