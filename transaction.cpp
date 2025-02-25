#include <iostream>
#include <ctime>
#include<memory>

class TxnPayload{

};

class moneyTxn : public TxnPayload{
    float amount;
    std::string to_hash;
};
class stakeTxn : public TxnPayload{
    float amount;
};
class smartcontractTxn: public TxnPayload{
    std::string contract_addr;
};
class transcaction
{
private:
    char type;
    std::string from_hash;
    float gas_fee;
    std::unique_ptr<TxnPayload> payload;
    std::time_t timestamp;
    std::string sig;
    std::string valid;
    std::string txn_hash;
};