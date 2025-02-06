#pragma once
#include <unordered_map>
#include <shared_mutex>
#include <optional>
#include <string>

class KeyValueStore {
private:
    std::unordered_map<std::string, std::string> data;
    mutable std::shared_mutex mutex;

public:
    void put(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key) const;
    void remove(const std::string& key);
};