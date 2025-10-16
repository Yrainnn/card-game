#pragma once

#include "Classes/models/CardEnums.h"

namespace cardgame {

class CardMatchService {
public:
    static bool isMatchable(CardFaceType trayFace, CardFaceType targetFace);
};

} // namespace cardgame
