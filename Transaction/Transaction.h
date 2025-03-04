#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <vector>
#include <string>
#include "TXInput.h"
#include "TXOutput.h"

class Transaction
{
public:
    std::string txID;
    std::vector<TxInput> inputs;
    std::vector<TxOutput> outputs;

    Transaction(std::vector<TxInput> inputs, std::vector<TxOutput> outputs);
    std::string calculateTxID();
};

#endif
