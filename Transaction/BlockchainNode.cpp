#include "BlockchainNode.h"

void BlockchainNode::requestTransaction(const std::string &txID)
{
    std::cout << "Requesting missing transaction: " << txID << " from peers...\n";
}

void BlockchainNode::processTransaction(const Transaction &tx)
{
    for (const auto &input : tx.inputs)
    {
        if (!utxoSet.isUTXOAvailable(input.txID, input.index))
        {
            std::cout << "UTXO not found! Requesting transaction...\n";
            requestTransaction(input.txID);
            return;
        }
    }

    for (const auto &input : tx.inputs)
    {
        utxoSet.removeUTXO(input.txID, input.index);
        // need to broadcast to other full nodes
    }

    for (size_t i = 0; i < tx.outputs.size(); i++)
    {
        utxoSet.addUTXO(tx.txID, i, tx.outputs[i].amount);
        // need to broadcast to other full nodes
    }

    broadcastUTXOUpdate();
    std::cout << "Transaction successfully processed!\n";
}

void BlockchainNode::broadcastUTXOUpdate()
{
    std::unordered_set<std::pair<std::string, int>, pair_hash> spentUTXOs = utxoSet.getSpentUTXOs();
    std::cout << "Broadcasting spent UTXOs to peers...\n";
    for (const auto &[txID, index] : spentUTXOs)
    {
        std::cout << " - Spent UTXO: " << txID << ":" << index << "\n";
    }
}

void BlockchainNode::syncWithPeers(BlockchainNode &peer)
{
    std::cout << "Synchronizing UTXOs with peer...\n";
    utxoSet.syncSpentUTXOs(peer.utxoSet.getSpentUTXOs());
}
