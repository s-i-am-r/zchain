
void test()
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
