#include <iostream>
#include <openssl/evp.h>
#include <iomanip>
#include <sstream>
#include <string>
#include<vector>
#include<memory>
std::string sha256(const std::string& str) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) throw std::runtime_error("Failed to create EVP_MD_CTX");

    const EVP_MD* md = EVP_sha256();
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int length;

    if (1 != EVP_DigestInit_ex(ctx, md, nullptr)) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("EVP_DigestInit_ex failed");
    }

    if (1 != EVP_DigestUpdate(ctx, str.c_str(), str.size())) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("EVP_DigestUpdate failed");
    }

    if (1 != EVP_DigestFinal_ex(ctx, hash, &length)) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("EVP_DigestFinal_ex failed");
    }

    EVP_MD_CTX_free(ctx);

    std::stringstream ss;
    for (unsigned int i = 0; i < length; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return ss.str();
}

using namespace std;
class z_block{
private:
    std::string data;
    std::string prev_hash;
    std::string cur_hash;
public:
    string get_data(){
        return data;
    }
    string get_p_hash(){
        return prev_hash;
    }
    string get_c_hash(){
        return cur_hash;
    }
    z_block(string data,string prev_hash ){
        this->data=data;
        this->prev_hash=prev_hash;
        this->cur_hash=sha256(data+prev_hash);
    }
    
};

class z_chain{
    private:
        std::vector<shared_ptr<z_block>> blocks;
    public:
        string top_hash(){
            return blocks[blocks.size()-1]->get_c_hash();
        }
        z_chain(){
            string d="sriram.";
            string h="0";
            z_block gen(d,h);
            add_block(gen);
        }
        void add_block( z_block& blk){
            blocks.push_back(make_shared<z_block>(blk));
        }
        void print_chain(){
            for (const auto& blk:blocks)
            {
                cout<<"---\ndata: "<<blk->get_data()<<"\nprev hash: "<<blk->get_p_hash()<<"\n---"<<endl;
            }
        }
};



int main(){
    z_chain myfirst;
    string x;
    cin>>x;
    z_block tmp(x,myfirst.top_hash());
    myfirst.add_block(tmp);
    myfirst.print_chain();
}
