#include "TXInput.h"

TxInput::TxInput(std::string txID, int index, std::string signature)
    : txID(txID), index(index), signature(signature) {}
