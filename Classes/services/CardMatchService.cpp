#include "Classes/services/CardMatchService.h"

#include <cmath>

namespace cardgame {

bool CardMatchService::isMatchable(CardFaceType trayFace, CardFaceType targetFace) {
    if (trayFace == CardFaceType::CFT_JOKER || targetFace == CardFaceType::CFT_JOKER) {
        return true;
    }

    const int trayVal = static_cast<int>(trayFace);
    const int targetVal = static_cast<int>(targetFace);

    if (trayFace == CardFaceType::CFT_ACE) {
        return targetFace == CardFaceType::CFT_TWO || targetFace == CardFaceType::CFT_KING;
    }
    if (trayFace == CardFaceType::CFT_KING) {
        return targetFace == CardFaceType::CFT_QUEEN || targetFace == CardFaceType::CFT_ACE;
    }

    return std::abs(trayVal - targetVal) == 1;
}

} // namespace cardgame
