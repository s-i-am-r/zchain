#include <iostream>
#include <vector>
#include <ctime>
#include "block.h"
#include "chain.cpp"
#include "Transaction/Transaction.h"
#include "Transaction/BlockchainNode.h"
#include "Transaction/TXInput.h"
#include "Transaction/TXOutput.h"
#include "Transaction/utxo.h"
#include "Transaction/UTXOSet.h"

void testBlockchain()
{
    std::vector<std::string> blockData = {
        "Randomdata1",
        "Randomdata2",
        "Randomdata3",
        "Randomdata4"};

    std::cout << "Test Case 1: Create a chain and add blocks\n";
    z_chain myChain;
    myChain.print_chain();

    for (const auto &data : blockData)
    {
        z_block newBlock(data, myChain.top_hash(), std::time(nullptr));
        myChain.add_block(newBlock);
    }

    myChain.print_chain();

    std::cout << "\nTest Case 2: Save chain to a file\n";
    std::string filename = "z_chain.txt";
    myChain.save_to_file(filename);

    std::cout << "Chain saved to file: " << filename << "\n";

    std::cout << "\nTest Case 3: Retrieve chain from file\n";

    z_chain newChain;
    newChain.retrive_from_file(filename);
    newChain.print_chain();
}

void testTransactions()
{
    BlockchainNode node;

    // Example transaction
    Transaction tx1(
        {TxInput("123456", 0, "AliceSignature")},
        {TxOutput(0.6, "Bob"), TxOutput(0.4, "Alice")});

    node.processTransaction(tx1);

    // Another transaction
    Transaction tx2(
        {TxInput(tx1.txID, 0, "BobSignature")},
        {TxOutput(0.3, "Charlie"), TxOutput(0.3, "Bob")});

    node.processTransaction(tx2);
}

int main()
{
    std::cout << "Running blockchain tests...\n";
    testBlockchain();

    std::cout << "\nRunning transaction tests...\n";
    testTransactions();

    return 0;
}