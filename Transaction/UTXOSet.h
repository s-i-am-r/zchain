#ifndef UTXOSET_H
#define UTXOSET_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <utility>
#include "utxo.h"

// Custom hash function for pair<string, int>
struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &p) const
    {
        return std::hash<T1>{}(p.first) ^ (std::hash<T2>{}(p.second) << 1);
    }
};

class UTXOSet
{
private:
    std::unordered_map<std::pair<std::string, int>, UTXO, pair_hash> utxos;
    std::unordered_set<std::pair<std::string, int>, pair_hash> spentUTXOs;

public:
    void addUTXO(const std::string &txID, int index, double amount);
    bool removeUTXO(const std::string &txID, int index);
    bool isUTXOAvailable(const std::string &txID, int index) const;
    std::unordered_set<std::pair<std::string, int>, pair_hash> getSpentUTXOs() const;
    void syncSpentUTXOs(const std::unordered_set<std::pair<std::string, int>, pair_hash> &remoteSpentUTXOs);
    std::unordered_map<std::pair<std::string, int>, UTXO, pair_hash> getUTXOs() const;
};

#endif
