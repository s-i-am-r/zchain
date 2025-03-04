#include "utxo.h"

UTXO::UTXO() : txID(""), index(0), amount(0.0) {}

UTXO::UTXO(const std::string &txID, int index, double amount)
    : txID(txID), index(index), amount(amount) {}

std::string UTXO::getKey() const
{
    return txID + ":" + std::to_string(index);
}
