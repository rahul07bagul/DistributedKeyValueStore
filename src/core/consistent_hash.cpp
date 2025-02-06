#include "consistent_hash.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>

void ConsistentHashRing::addNode(const std::string& node) {
    if (node.empty()) {
        throw std::invalid_argument("Node identifier cannot be empty");
    }
    for (int i = 0; i < VIRTUAL_NODES; ++i) {
        std::string vnode = node + "#" + std::to_string(i);
        hash_ring[hash(vnode)] = node;
    }
}


void ConsistentHashRing::removeNode(const std::string& node) {
    if (node.empty()) {
        throw std::invalid_argument("Node identifier cannot be empty");
    }
    for (int i = 0; i < VIRTUAL_NODES; ++i) {
        std::string vnode = node + "#" + std::to_string(i);
        hash_ring.erase(hash(vnode));
    }
}

std::string ConsistentHashRing::getNode(const std::string& key) {
    if (hash_ring.empty()) {
        throw std::runtime_error("Hash ring is empty");
    }
    if (key.empty()) {
        throw std::invalid_argument("Key cannot be empty");
    }
    size_t h = hash(key);
    auto it = hash_ring.lower_bound(h);
    if (it == hash_ring.end()) {
        return hash_ring.begin()->second;
    }
    return it->second;
}

std::vector<std::string> ConsistentHashRing::getSuccessors(const std::string& node_id, int num_replicas) {
    std::vector<std::string> successors;
    if (hash_ring.empty() || num_replicas <= 0) {
        return successors;
    }
    std::vector<std::pair<size_t, std::string>> ringList(hash_ring.begin(), hash_ring.end());
    std::sort(ringList.begin(), ringList.end(), [](auto &a, auto &b) {
        return a.first < b.first;
    });
    int startIndex = -1;
    for (int i = 0; i < (int)ringList.size(); i++) {
        if (ringList[i].second == node_id) {
            startIndex = i;
            break;
        }
    }
    if (startIndex < 0) {
        return successors;
    }
    int found = 0;
    int idx = startIndex;
    while (found < num_replicas) {
        idx = (idx + 1) % ringList.size();
        if (ringList[idx].second != node_id) {
            if (std::find(successors.begin(), successors.end(), ringList[idx].second) == successors.end()) {
                successors.push_back(ringList[idx].second);
                found++;
            }
        }
        if (idx == startIndex) {
            break;
        }
    }
    return successors;
}