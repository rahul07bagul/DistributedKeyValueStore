#include "key_value_store.h"
#include <iostream>
#include <shared_mutex>
#include <mutex>

void KeyValueStore::put(const std::string& key, const std::string& value) {
    std::unique_lock<std::shared_mutex> lock(mutex);
    data[key] = value;
}

std::optional<std::string> KeyValueStore::get(const std::string& key) const {
    std::shared_lock<std::shared_mutex> lock(mutex);
    auto it = data.find(key);
    return it != data.end() ? std::optional<std::string>(it->second) : std::nullopt;
}

void KeyValueStore::remove(const std::string& key) {
    std::unique_lock<std::shared_mutex> lock(mutex);
    data.erase(key);
}