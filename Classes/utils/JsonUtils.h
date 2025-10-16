#pragma once

#include <string>

#include "cocos2d.h"

namespace cardgame {

class JsonUtils {
public:
    static cocos2d::Value readJsonFile(const std::string& filePath);
    static cocos2d::Value parseJsonString(const std::string& jsonStr);
    static std::string valueMapToJsonString(const cocos2d::ValueMap& valueMap);

    static int getIntFromJson(const cocos2d::ValueMap& jsonMap, const std::string& key, int defaultVal = 0);
    static float getFloatFromJson(const cocos2d::ValueMap& jsonMap, const std::string& key, float defaultVal = 0.0f);
    static bool getBoolFromJson(const cocos2d::ValueMap& jsonMap, const std::string& key, bool defaultVal = false);
    static cocos2d::Vec2 getVec2FromJson(const cocos2d::ValueMap& jsonMap,
                                         const std::string& keyX,
                                         const std::string& keyY,
                                         const cocos2d::Vec2& defaultVal = cocos2d::Vec2::ZERO);
};

} // namespace cardgame
