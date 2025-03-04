#include "BlockchainNode.h"

int main()
{
    BlockchainNode node;

    // Example transaction
    Transaction tx1(
        {TxInput("123456", 0, "AliceSignature")},
        {TxOutput(0.6, "Bob"), TxOutput(0.4, "Alice")});

    node.processTransaction(tx1);
    return 0;
}
