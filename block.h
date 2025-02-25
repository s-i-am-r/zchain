#if !defined(BLOCK_H)
#define BLOCK_H
#include <string>
#include <openssl/evp.h>
#include <string>
#include <iomanip>
std::string sha256(const std::string &str);
class z_block
{
private:
    std::string data;
    std::string prev_hash;
    std::string cur_hash;
    std::time_t timestamp;
    int nonce;

public:
    std::string get_data();
    std::string get_p_hash();
    std::string get_c_hash();
    std::time_t get_timestamp();
    z_block(std :: string data, std::string prev_hash, std::time_t timestamp);
    z_block(std::string &data, std::string &prev_hash, std::string &curr_hash, std::time_t timestamp);
};

#endif // BLOCK_H
// This is bogus