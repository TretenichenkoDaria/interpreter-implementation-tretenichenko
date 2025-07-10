#pragma once
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
    virtual double evaluate(Context& ctx) const = 0;
    virtual ~Expression() = default;
};

using ExprPtr = unique_ptr<Expression>;

struct Number : Expression
{
    double value;
    Number(double v) : value(v) {}
    double evaluate(Context&) const override { return value; }
};

struct BinaryOp : Expression
{
    char operation;
    ExprPtr left, right;

    BinaryOp(char op, ExprPtr l, ExprPtr r) : operation(op), left(move(l)), right(move(r)) {}

    double evaluate(Context& ctx) const override;
};

struct FunctionCall : Expression
{
    string name;
    vector<ExprPtr> args;

    FunctionCall(string name, vector<ExprPtr> args) : name(move(name)), args(move(args)) {}

    double evaluate(Context& ctx) const override;
};

struct Variable : Expression
{
    string name;

    Variable(string name) : name(move(name)) {}

    double evaluate(Context& ctx) const override;
};

struct Assignment : Expression
{
    string name;
    ExprPtr value;

    Assignment(string name, ExprPtr val) : name(move(name)), value(move(val)) {}

    double evaluate(Context& ctx) const override;
};

