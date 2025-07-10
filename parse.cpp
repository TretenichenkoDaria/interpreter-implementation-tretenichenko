#include "ast.hpp"

#include <cctype>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

ExprPtr parse(const string& input);

static void skipSpaces(const string& input, size_t& position)
{
    while (position < input.size() && isspace(input[position])) position++;
}

static double parseNumber(const string& input, size_t& position)
{
    skipSpaces(input, position);
    size_t start = position;
    while (position < input.size() && (isdigit(input[position]) || input[position] == '.'))
    {
        position++;
    }
    if (start == position)
    {
        throw runtime_error("expected number");
    }
    return stod(input.substr(start, position - start));
}

static string parseIdentifier(const string& input, size_t& position)
{
    skipSpaces(input, position);
    size_t start = position;
    if (!isalpha(input[position]))
    {
        throw runtime_error("expected identifier");
    }
    while (position < input.size() && (isalnum(input[position]) || input[position] == '_'))
    {
        position++;
    }
    return input.substr(start, position - start);
}

ExprPtr parseExpression(const string& input, size_t& position);
ExprPtr parseTerm(const string& input, size_t& position);
ExprPtr parseFactor(const string& input, size_t& position);

ExprPtr parseExpression(const string& input, size_t& position)
{
    ExprPtr left = parseTerm(input, position);
    skipSpaces(input, position);

    while (position < input.size() && (input[position] == '+' || input[position] == '-'))
    {
        char op = input[position++];
        ExprPtr right = parseTerm(input, position);
        left = make_unique<BinaryOp>(op, move(left), move(right));
        skipSpaces(input, position);
    }

    return left;
}

ExprPtr parseTerm(const string& input, size_t& position)
{
    ExprPtr left = parseFactor(input, position);
    skipSpaces(input, position);

    while (position < input.size() && (input[position] == '*' || input[position] == '/'))
    {
        char op = input[position++];
        ExprPtr right = parseFactor(input, position);
        left = make_unique<BinaryOp>(op, move(left), move(right));
        skipSpaces(input, position);
    }

    return left;
}

ExprPtr parseFactor(const string& input, size_t& position)
{
    skipSpaces(input, position);

    if (input[position] == '-' || input[position] == '+')
    {
        char sign = input[position++];
        ExprPtr factor = parseFactor(input, position);
        if (sign == '-')
        {
            return make_unique<BinaryOp>('*', make_unique<Number>(-1), move(factor));
        }
        else
        {
            return factor;
        }
    }

    if (input[position] == '(')
        {
        position++;
        ExprPtr expr = parseExpression(input, position);
        skipSpaces(input, position);
        if (position >= input.size() || input[position] != ')')
        {
            throw runtime_error("expected )");
        }
        position++;
        return expr;
    }

    if (isalpha(input[position]))
    {
        string name = parseIdentifier(input, position);
        skipSpaces(input, position);

        if (position < input.size() && input[position] == '(')
        {
            position++;
            vector<ExprPtr> args;

            skipSpaces(input, position);
            if (input[position] != ')')
            {
                while (true)
                {
                    args.push_back(parseExpression(input, position));
                    skipSpaces(input, position);
                    if (input[position] == ')')
                    {
                        break;
                    }
                    if (input[position] != ',')
                    {
                        throw runtime_error("expected , or )");
                    }
                    position++;
                }
            }

            if (input[position] != ')')
            {
                throw runtime_error("expected )");
            }
            position++;

            return make_unique<FunctionCall>(name, move(args));
        }

        return make_unique<Variable>(name);
    }

    return make_unique<Number>(parseNumber(input, position));
}

ExprPtr parse(const string& input)
{
    size_t position = 0;
    skipSpaces(input, position);

    if (input.compare(position, 3, "var") == 0)
        {
        position += 3;
        skipSpaces(input, position);
        string name = parseIdentifier(input, position);
        skipSpaces(input, position);
        if (input[position] != '=')
        {
            throw runtime_error("expected = after variable name");
        }

        position++;
        ExprPtr val = parseExpression(input, position);
        skipSpaces(input, position);
        if (position != input.length())
        {
            throw runtime_error("unexpected characters after assignment");
        }
        return make_unique<Assignment>(name, move(val));
    }

    ExprPtr result = parseExpression(input, position);
    skipSpaces(input, position);
    if (position != input.length())
    {
        throw runtime_error("unexpected characters after expression");
    }
    return result;
}
