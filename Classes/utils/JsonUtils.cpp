#include "Classes/utils/JsonUtils.h"

#include <sstream>

namespace cardgame {
namespace {

std::string escapeString(const std::string& input) {
    std::ostringstream oss;
    for (char ch : input) {
        switch (ch) {
        case '"':
            oss << "\\\"";
            break;
        case '\\':
            oss << "\\\\";
            break;
        case '\n':
            oss << "\\n";
            break;
        case '\r':
            oss << "\\r";
            break;
        case '\t':
            oss << "\\t";
            break;
        default:
            oss << ch;
            break;
        }
    }
    return oss.str();
}

void appendValueToStream(std::ostringstream& oss, const cocos2d::Value& value) {
    switch (value.getType()) {
    case cocos2d::Value::Type::MAP: {
        oss << "{";
        bool first = true;
        for (const auto& entry : value.asValueMap()) {
            if (!first) {
                oss << ",";
            }
            first = false;
            oss << "\"" << escapeString(entry.first) << "\":";
            appendValueToStream(oss, entry.second);
        }
        oss << "}";
        break;
    }
    case cocos2d::Value::Type::VECTOR: {
        oss << "[";
        bool first = true;
        for (const auto& element : value.asValueVector()) {
            if (!first) {
                oss << ",";
            }
            first = false;
            appendValueToStream(oss, element);
        }
        oss << "]";
        break;
    }
    case cocos2d::Value::Type::STRING:
        oss << "\"" << escapeString(value.asString()) << "\"";
        break;
    case cocos2d::Value::Type::INTEGER:
        oss << value.asInt();
        break;
    case cocos2d::Value::Type::FLOAT:
    case cocos2d::Value::Type::DOUBLE:
        oss << value.asDouble();
        break;
    case cocos2d::Value::Type::BOOLEAN:
        oss << (value.asBool() ? "true" : "false");
        break;
    default:
        oss << "null";
        break;
    }
}

} // namespace

cocos2d::Value JsonUtils::readJsonFile(const std::string& filePath) {
    cocos2d::ValueMap map = cocos2d::FileUtils::getInstance()->getValueMapFromFile(filePath);
    return cocos2d::Value(map);
}

cocos2d::Value JsonUtils::parseJsonString(const std::string& jsonStr) {
    cocos2d::ValueMap map =
        cocos2d::FileUtils::getInstance()->getValueMapFromData(jsonStr.c_str(), jsonStr.size());
    return cocos2d::Value(map);
}

std::string JsonUtils::valueMapToJsonString(const cocos2d::ValueMap& valueMap) {
    std::ostringstream oss;
    appendValueToStream(oss, cocos2d::Value(valueMap));
    return oss.str();
}

int JsonUtils::getIntFromJson(const cocos2d::ValueMap& jsonMap, const std::string& key, int defaultVal) {
    auto iter = jsonMap.find(key);
    if (iter == jsonMap.end()) {
        return defaultVal;
    }
    if (iter->second.getType() == cocos2d::Value::Type::INTEGER) {
        return iter->second.asInt();
    }
    if (iter->second.getType() == cocos2d::Value::Type::DOUBLE ||
        iter->second.getType() == cocos2d::Value::Type::FLOAT) {
        return static_cast<int>(iter->second.asDouble());
    }
    return defaultVal;
}

float JsonUtils::getFloatFromJson(const cocos2d::ValueMap& jsonMap,
                                  const std::string& key,
                                  float defaultVal) {
    auto iter = jsonMap.find(key);
    if (iter == jsonMap.end()) {
        return defaultVal;
    }
    if (iter->second.getType() == cocos2d::Value::Type::FLOAT ||
        iter->second.getType() == cocos2d::Value::Type::DOUBLE) {
        return iter->second.asFloat();
    }
    if (iter->second.getType() == cocos2d::Value::Type::INTEGER) {
        return static_cast<float>(iter->second.asInt());
    }
    return defaultVal;
}

bool JsonUtils::getBoolFromJson(const cocos2d::ValueMap& jsonMap, const std::string& key, bool defaultVal) {
    auto iter = jsonMap.find(key);
    if (iter == jsonMap.end()) {
        return defaultVal;
    }
    if (iter->second.getType() == cocos2d::Value::Type::BOOLEAN) {
        return iter->second.asBool();
    }
    return defaultVal;
}

cocos2d::Vec2 JsonUtils::getVec2FromJson(const cocos2d::ValueMap& jsonMap,
                                         const std::string& keyX,
                                         const std::string& keyY,
                                         const cocos2d::Vec2& defaultVal) {
    float x = defaultVal.x;
    float y = defaultVal.y;
    auto iterX = jsonMap.find(keyX);
    if (iterX != jsonMap.end()) {
        x = iterX->second.asFloat();
    }
    auto iterY = jsonMap.find(keyY);
    if (iterY != jsonMap.end()) {
        y = iterY->second.asFloat();
    }
    return cocos2d::Vec2(x, y);
}

} // namespace cardgame
