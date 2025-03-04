#include "Transaction.h"
#include <sstream>
#include <functional>

Transaction::Transaction(std::vector<TxInput> inputs, std::vector<TxOutput> outputs)
    : inputs(inputs), outputs(outputs)
{
    txID = calculateTxID();
}

std::string Transaction::calculateTxID()
{
    std::ostringstream ss;
    for (auto &in : inputs)
        ss << in.txID << in.index;
    for (auto &out : outputs)
        ss << out.amount << out.recipientAddress;
    return std::to_string(std::hash<std::string>{}(ss.str()));
}
