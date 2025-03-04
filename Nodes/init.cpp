#include <bits/stdc++.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/rand.h>

class zNode
{
private:
    std::string public_key, private_key; // fixed length

    // wallet connection -> (currency , nft details related to market place)
    // peers information
public:
    void generateKeyPair()
    {
        const int keyLength = 2048;

        // Generate a strong random seed
        unsigned char randSeed[32]; // 256-bit seed
        if (RAND_bytes(randSeed, sizeof(randSeed)) != 1)
        {
            std::cerr << "Error generating cryptographic random seed!" << std::endl;
            return;
        }

        RAND_seed(randSeed, sizeof(randSeed)); // Seed OpenSSL RNG

        EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_from_name(nullptr, "RSA", nullptr);
        if (!ctx)
        {
            std::cerr << "Error: Failed to create EVP_PKEY context!" << std::endl;
            return;
        }

        if (EVP_PKEY_keygen_init(ctx) <= 0)
        {
            std::cerr << "Error initializing key generation!" << std::endl;
            EVP_PKEY_CTX_free(ctx);
            return;
        }

        if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, keyLength) <= 0)
        {
            std::cerr << "Error setting RSA key length!" << std::endl;
            EVP_PKEY_CTX_free(ctx);
            return;
        }

        // Generate the key pair
        EVP_PKEY *pkey = nullptr;
        if (EVP_PKEY_generate(ctx, &pkey) <= 0)
        {
            std::cerr << "Error generating RSA key pair!" << std::endl;
            EVP_PKEY_CTX_free(ctx);
            return;
        }

        EVP_PKEY_CTX_free(ctx);

        // Save Private Key
        std::ofstream privFile("private_key.pem");
        if (privFile)
        {
            BIO *privBio = BIO_new(BIO_s_mem());
            PEM_write_bio_PrivateKey(privBio, pkey, nullptr, nullptr, 0, nullptr, nullptr);

            char *privKeyData;
            long privLen = BIO_get_mem_data(privBio, &privKeyData);
            privFile.write(privKeyData, privLen);
            privFile.close();
            BIO_free(privBio);
            std::cout << "✅ Private key saved to private_key.pem" << std::endl;
        }

        std::ofstream pubFile("public_key.pem");
        if (pubFile)
        {
            BIO *pubBio = BIO_new(BIO_s_mem());
            PEM_write_bio_PUBKEY(pubBio, pkey);

            char *pubKeyData;
            long pubLen = BIO_get_mem_data(pubBio, &pubKeyData);
            pubFile.write(pubKeyData, pubLen);
            pubFile.close();
            BIO_free(pubBio);
            std::cout << "✅ Public key saved to public_key.pem" << std::endl;
        }

        // cleanupp  
        EVP_PKEY_free(pkey);
    }
};

int main()
{
    std::cout << "🔹 Generating RSA Key Pair using Cryptographic Randomness... and once lost cannot be retrived !!!\n";
    zNode z;
    z.generateKeyPair();
    return 0;
}

// block is mined say 2 min once at that time current staker are selected based upon weighted probability .......