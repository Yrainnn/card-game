#pragma once

#include <string>
#include <unordered_map>

#include "Classes/models/CardEnums.h"

namespace cardgame {

class CardResConfig {
public:
    CardResConfig();

    std::string getCardResPath(CardSuitType suit, CardFaceType face) const;

private:
    std::unordered_map<std::string, std::string> _cardResMap;

    static std::string buildKey(CardSuitType suit, CardFaceType face);
};

} // namespace cardgame
