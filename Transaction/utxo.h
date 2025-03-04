#ifndef UTXO_H
#define UTXO_H

#include <string>

class UTXO
{
private:
    std::string txID;
    int index;
    double amount;

public:
    UTXO(); // Default constructor
    UTXO(const std::string &txID, int index, double amount);

    std::string getKey() const;

    // Getter methods
    std::string getTxID() const { return txID; }
    int getIndex() const { return index; }
    double getAmount() const { return amount; }
};

#endif
