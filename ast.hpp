// ast.hpp
#pragma once
#include <memory>
#include <string>
#include "context.hpp"

struct Expression {
    virtual double evaluate(Context& ctx) const = 0;
    virtual ~Expression() = default;
};

using ExprPtr = std::unique_ptr<Expression>;

struct Number : public Expression {
    double value;
    Number(double v) : value(v) {}
    double evaluate(Context&) const override { return value; }
};

struct BinaryOp : public Expression {
    char op;
    ExprPtr left, right;

    BinaryOp(char op, ExprPtr l, ExprPtr r)
        : op(op), left(std::move(l)), right(std::move(r)) {}

    double evaluate(Context& ctx) const override;
};

struct FunctionCall : public Expression {
    std::string name;
    std::vector<ExprPtr> args;

    FunctionCall(std::string name, std::vector<ExprPtr> args)
        : name(std::move(name)), args(std::move(args)) {}

    double evaluate(Context& ctx) const override;
};

struct Variable : public Expression {
    std::string name;

    Variable(std::string name) : name(std::move(name)) {}

    double evaluate(Context& ctx) const override;
};

struct Assignment : public Expression {
    std::string name;
    ExprPtr value;

    Assignment(std::string name, ExprPtr val)
        : name(std::move(name)), value(std::move(val)) {}

    double evaluate(Context& ctx) const override;
};

