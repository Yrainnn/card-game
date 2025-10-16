#include "Classes/models/UndoModel.h"

namespace cardgame {

UndoModel::UndoModel(UndoOperationType opType,
                     int preTrayId,
                     std::unordered_map<int, CardStateSnapshot> cardSnapshots,
                     std::vector<int> stackCardIdsBefore)
    : _operationType(opType)
    , _cardSnapshots(std::move(cardSnapshots))
    , _preTrayCardId(preTrayId)
    , _stackCardIdsBefore(std::move(stackCardIdsBefore)) {}

UndoOperationType UndoModel::getOperationType() const { return _operationType; }

const std::unordered_map<int, CardStateSnapshot>& UndoModel::getCardSnapshots() const {
    return _cardSnapshots;
}

int UndoModel::getPreTrayCardId() const { return _preTrayCardId; }

const std::vector<int>& UndoModel::getStackCardIdsBefore() const {
    return _stackCardIdsBefore;
}

} // namespace cardgame
