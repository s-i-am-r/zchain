#ifndef BLOCKCHAINNODE_H
#define BLOCKCHAINNODE_H

#include "Transaction.h"
#include "UTXOSet.h"
#include <iostream>

class BlockchainNode
{
private:
    UTXOSet utxoSet;

public:
    void requestTransaction(const std::string &txID);
    void processTransaction(const Transaction &tx);
    void broadcastUTXOUpdate();
    void syncWithPeers(BlockchainNode &peer);
};

#endif
