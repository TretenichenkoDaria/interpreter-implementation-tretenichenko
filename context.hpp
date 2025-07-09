#pragma once
#include <string>
#include <unordered_map>

struct Context {
    std::unordered_map<std::string, double> variables;
};
