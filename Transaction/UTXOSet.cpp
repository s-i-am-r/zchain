#include "UTXOSet.h"

void UTXOSet::addUTXO(const std::string &txID, int index, double amount)
{
    UTXO utxo(txID, index, amount);
    utxos[std::make_pair(txID, index)] = utxo; // Changed to use std::pair as key
}

bool UTXOSet::removeUTXO(const std::string &txID, int index)
{
    auto key = std::make_pair(txID, index);
    auto it = utxos.find(key);

    if (it != utxos.end())
    {
        utxos.erase(it);
        spentUTXOs.insert(key); // Ensure spent UTXOs are stored correctly
        return true;
    }
    return false;
}

bool UTXOSet::isUTXOAvailable(const std::string &txID, int index) const
{
    return utxos.find(std::make_pair(txID, index)) != utxos.end();
}

std::unordered_set<std::pair<std::string, int>, pair_hash> UTXOSet::getSpentUTXOs() const
{
    return spentUTXOs;
}

void UTXOSet::syncSpentUTXOs(const std::unordered_set<std::pair<std::string, int>, pair_hash> &remoteSpentUTXOs)
{
    for (const auto &key : remoteSpentUTXOs)
    {
        utxos.erase(key);
        spentUTXOs.insert(key);
    }
}
