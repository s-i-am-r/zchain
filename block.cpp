
#include "block.h"

std::string sha256(const std::string &str)
{
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx)
        throw std::runtime_error("Failed to create EVP_MD_CTX");

    const EVP_MD *md = EVP_sha256();
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int length;

    if (1 != EVP_DigestInit_ex(ctx, md, nullptr))
    {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("EVP_DigestInit_ex failed");
    }

    if (1 != EVP_DigestUpdate(ctx, str.c_str(), str.size()))
    {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("EVP_DigestUpdate failed");
    }

    if (1 != EVP_DigestFinal_ex(ctx, hash, &length))
    {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("EVP_DigestFinal_ex failed");
    }

    EVP_MD_CTX_free(ctx);

    std::stringstream ss;
    for (unsigned int i = 0; i < length; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return ss.str();
}

std::string z_block::get_data()
{
    return data;
}
std::string z_block::get_p_hash()
{
    return prev_hash;
}
std::string z_block::get_c_hash()
{
    return cur_hash;
}
std::time_t z_block::get_timestamp()
{
    return timestamp;
}
z_block::z_block(std::string data, std::string prev_hash, std::time_t timestamp)
{
    this->data = data;
    this->prev_hash = prev_hash;
    this->timestamp = timestamp;
    this->nonce = 0;
    this->cur_hash = sha256(data + prev_hash + std::to_string(this->timestamp)); // Add nonce if req
}
// For copying the blocks that are retrived from a file
z_block::z_block(std::string &data, std::string &prev_hash, std::string &curr_hash, std::time_t timestamp)
{
    this->data = data;
    this->prev_hash = prev_hash;
    this->cur_hash = curr_hash;
    this->timestamp = timestamp;
    this->nonce = 0;
    this->cur_hash = sha256(data + prev_hash + std::to_string(this->timestamp)); // Add nonce if req
}
