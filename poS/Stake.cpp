#include <iostream>
#include <string>
#include <json/json.h>
#include <fstream>
#include <vector>
#include <random>


// Here the stake is fixed

class ProofOfStake {
public:
    std::string PickWinner() {
        Json::Value jdata;
        std::ifstream inputfile("stakes.json");

        if (!inputfile.is_open()) {
            std::cerr << "Could not open the file!" << std::endl;
            return "";
        }

        try {
            inputfile >> jdata;
        } catch (const Json::RuntimeError& e) {
            std::cerr << "JSON parsing error: " << e.what() << std::endl;
            return "";
        }

        inputfile.close();

        std::vector<std::string> lotteryPool;

        for (const auto& validator : jdata["validators"]) {
            std::string address = validator["address"].asString();
            int stake = validator["stake"].asInt();

            for (int i = 0; i < stake; ++i) {
                lotteryPool.push_back(address);
            }
        }

        if (lotteryPool.empty()) {
            std::cerr << "No validators available to pick a winner." << std::endl;
            return "";
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, lotteryPool.size() - 1);
        std::string winner = lotteryPool[dis(gen)];

        return winner;
    }
};

int main() {
    ProofOfStake pos;
    std::string winner = pos.PickWinner();
    if (!winner.empty()) {
        std::cout << "The winner is: " << winner << std::endl;
    }
    return 0;
}