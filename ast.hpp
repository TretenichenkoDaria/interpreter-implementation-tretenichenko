#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

struct Context
{
    unordered_map<string, double> variables;
};

struct Expression
{
    virtual double evaluate(Context& context) = 0;
    virtual ~Expression() = default;
};

using ExpressionPointer = unique_ptr<Expression>;

struct Number : Expression
{
    double value;
    Number(double value) : value(value) {}
    double evaluate(Context&) override { return value; }
};

struct BinaryOperation : Expression
{
    char operation;
    ExpressionPointer left, right;

    BinaryOperation(char operation, ExpressionPointer left, ExpressionPointer right) : operation(operation), left(move(left)), right(move(right)) {}

    double evaluate(Context& context) override;
};

struct FunctionCall : Expression
{
    string name;
    vector<ExpressionPointer> args;

    FunctionCall(string name, vector<ExpressionPointer> args) : name(move(name)), args(move(args)) {}

    double evaluate(Context& context) override;
};

struct Variable : Expression
{
    string name;

    Variable(string name) : name(move(name)) {}

    double evaluate(Context& context) override;
};

struct Assignment : Expression
{
    string name;
    ExpressionPointer value;

    Assignment(string name, ExpressionPointer value) : name(move(name)), value(move(value)) {}

    double evaluate(Context& context) override;
};