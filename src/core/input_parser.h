#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <optional>

struct ParsedInput {
    std::string node_id;
    std::string address;
    std::unordered_map<std::string, std::string> peers;
};

class InputParser {
public:
    InputParser(int argc, char* argv[]);
    std::optional<ParsedInput> parse();
    static void print_usage();

private:
    std::vector<std::string> split(const std::string& s, char delimiter);
    bool parse_peers(const std::string& peers_str, std::unordered_map<std::string, std::string>& peers);

    int argc;
    char** argv;
};
