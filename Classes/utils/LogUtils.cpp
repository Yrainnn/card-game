#include "Classes/utils/LogUtils.h"

namespace cardgame {

void LogUtils::logDebug(const std::string& tag, const std::string& msg) {
#if COCOS2D_DEBUG
    cocos2d::log("[DEBUG][%s] %s", tag.c_str(), msg.c_str());
#endif
}

void LogUtils::logError(const std::string& tag, const std::string& msg) {
    cocos2d::log("[ERROR][%s] %s", tag.c_str(), msg.c_str());
}

} // namespace cardgame
