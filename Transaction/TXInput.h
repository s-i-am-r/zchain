#ifndef TXINPUT_H
#define TXINPUT_H

#include <string>

class TxInput {
public:
    std::string txID;
    int index;
    std::string signature;

    TxInput(std::string txID, int index, std::string signature);
};

#endif
