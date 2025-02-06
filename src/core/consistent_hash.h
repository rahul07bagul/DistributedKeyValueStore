#pragma once
#include <string>
#include <map>
#include <vector>
#include <openssl/sha.h>

class ConsistentHashRing {
public:
    static const int VIRTUAL_NODES = 100;
    void addNode(const std::string& node);
    void removeNode(const std::string& node);
    std::string getNode(const std::string& key);
    std::vector<std::string> getSuccessors(const std::string& node_id, int num_replicas);

private:
    std::map<size_t, std::string> hash_ring;
    size_t hash(const std::string& s) const {
        unsigned char hash[SHA_DIGEST_LENGTH];
        SHA1(reinterpret_cast<const unsigned char*>(s.c_str()), s.size(), hash);
        size_t result = 0;
        for (int i = 0; i < sizeof(size_t); i++) {
            result = (result << 8) | hash[i];
        }
        return result;
    }
};