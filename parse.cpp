#include "parser.hpp"
#include "ast.hpp"

#include <cctype>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

// ======== ДОПОМІЖНІ ФУНКЦІЇ ========

static void skipSpaces(const string& input, size_t& pos) {
    while (pos < input.size() && isspace(input[pos])) pos++;
}

static double parseNumber(const string& input, size_t& pos) {
    skipSpaces(input, pos);
    size_t start = pos;
    while (pos < input.size() && (isdigit(input[pos]) || input[pos] == '.')) pos++;
    if (start == pos) throw runtime_error("Expected number");
    return stod(input.substr(start, pos - start));
}

static string parseIdentifier(const string& input, size_t& pos) {
    skipSpaces(input, pos);
    size_t start = pos;
    if (!isalpha(input[pos])) throw runtime_error("Expected identifier");
    while (pos < input.size() && (isalnum(input[pos]) || input[pos] == '_')) pos++;
    return input.substr(start, pos - start);
}

// ======== FORWARD DECLARATIONS ========
ExprPtr parseExpression(const string& input, size_t& pos);
ExprPtr parseTerm(const string& input, size_t& pos);
ExprPtr parseFactor(const string& input, size_t& pos);

// ======== ВИРАЗИ: +, - ========
ExprPtr parseExpression(const string& input, size_t& pos) {
    ExprPtr left = parseTerm(input, pos);
    skipSpaces(input, pos);

    while (pos < input.size() && (input[pos] == '+' || input[pos] == '-')) {
        char op = input[pos++];
        ExprPtr right = parseTerm(input, pos);
        left = make_unique<BinaryOp>(op, move(left), move(right));
        skipSpaces(input, pos);
    }

    return left;
}

// ======== ТЕРМІНИ: *, / ========
ExprPtr parseTerm(const string& input, size_t& pos) {
    ExprPtr left = parseFactor(input, pos);
    skipSpaces(input, pos);

    while (pos < input.size() && (input[pos] == '*' || input[pos] == '/')) {
        char op = input[pos++];
        ExprPtr right = parseFactor(input, pos);
        left = make_unique<BinaryOp>(op, move(left), move(right));
        skipSpaces(input, pos);
    }

    return left;
}

// ======== ФАКТОРИ: числа, дужки, функції, змінні ========
ExprPtr parseFactor(const string& input, size_t& pos) {
    skipSpaces(input, pos);

    // Дужки
    if (input[pos] == '(') {
        pos++; // пропустити '('
        ExprPtr expr = parseExpression(input, pos);
        skipSpaces(input, pos);
        if (pos >= input.size() || input[pos] != ')')
            throw runtime_error("Expected ')'");
        pos++; // пропустити ')'
        return expr;
    }

    // Ідентифікатор (може бути функція або змінна)
    if (isalpha(input[pos])) {
        string name = parseIdentifier(input, pos);
        skipSpaces(input, pos);

        // Виклик функції
        if (pos < input.size() && input[pos] == '(') {
            pos++; // пропустити '('
            vector<ExprPtr> args;

            // обробка аргументів
            skipSpaces(input, pos);
            if (input[pos] != ')') {
                while (true) {
                    args.push_back(parseExpression(input, pos));
                    skipSpaces(input, pos);
                    if (input[pos] == ')') break;
                    if (input[pos] != ',') throw runtime_error("Expected ',' or ')'");
                    pos++; // пропустити ','
                }
            }

            if (input[pos] != ')') throw runtime_error("Expected ')'");
            pos++; // пропустити ')'
            return make_unique<FunctionCall>(name, move(args));
        }

        // Інакше – це змінна
        return make_unique<Variable>(name);
    }

    // Число
    return make_unique<Number>(parseNumber(input, pos));
}

// ======== ГОЛОВНА ТОЧКА ВХОДУ ========
ExprPtr parse(const std::string& input) {
    size_t pos = 0;
    skipSpaces(input, pos);

    // Перевірка на "var x = ..."
    if (input.compare(pos, 3, "var") == 0) {
        pos += 3;
        skipSpaces(input, pos);
        std::string name = parseIdentifier(input, pos);
        skipSpaces(input, pos);
        if (input[pos] != '=') throw std::runtime_error("Expected '=' after variable name");
        pos++; // пропустити '='
        ExprPtr val = parseExpression(input, pos);
        skipSpaces(input, pos);
        if (pos != input.length())
            throw std::runtime_error("Unexpected characters after assignment");
        return std::make_unique<Assignment>(name, std::move(val));
    }

    // Інакше — це просто вираз
    ExprPtr result = parseExpression(input, pos);
    skipSpaces(input, pos);
    if (pos != input.length()) {
        throw std::runtime_error("Unexpected characters after expression");
    }
    return result;
}
