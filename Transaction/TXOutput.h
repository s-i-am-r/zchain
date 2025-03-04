#ifndef TXOUTPUT_H
#define TXOUTPUT_H

#include <string>

class TxOutput
{
public:
    double amount;
    std::string recipientAddress;

    TxOutput(double amount, std::string recipientAddress);
};

#endif
