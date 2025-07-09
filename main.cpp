#include <iostream>
#include <string>
#include <memory>

#include "parser.hpp"
#include "context.hpp"

int main() {
    Context ctx;

    std::string line;
    std::cout << "> ";
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            std::cout << "> ";
            continue;
        }

        try {
            auto expr = parse(line);
            double result = expr->evaluate(ctx);
            std::cout << result << std::endl;
        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
        }

        std::cout << "> ";
    }

    return 0;
}
