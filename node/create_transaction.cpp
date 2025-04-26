#include <boost/json.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string read_file(const std::string &filename) {
    std::ifstream in(filename);
    std::stringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <sender_public_key_file>\n";
        return 1;
    }

    std::string sender_pub_key = read_file(argv[1]);
    std::string receiver_pub_key = read_file("receiver.pub");  // assumes fixed file
    std::string fake_signature = "deadbeefcafebabe001122334455";  // simulated signature

    // Read existing UTXO file (or create if not exists)
    std::ifstream in("utxo.json");
    boost::json::value root;

    if (in.is_open()) {
        std::stringstream buffer;
        buffer << in.rdbuf();
        in.close();

        if (!buffer.str().empty()) {
            root = boost::json::parse(buffer.str());
        }
    }

    if (!root.is_array()) {
        root = boost::json::array();  // initialize if file was empty or malformed
    }

    // Append new txn
    boost::json::object txn;
    txn["sender"] = sender_pub_key;
    txn["receiver"] = receiver_pub_key;
    txn["signature"] = fake_signature;

    root.as_array().push_back(txn);

    // Write back
    std::ofstream out("utxo.json");
    out << boost::json::serialize(root);
    out.close();

    std::cout << "Transaction appended to utxo.json\n";
    return 0;
}
