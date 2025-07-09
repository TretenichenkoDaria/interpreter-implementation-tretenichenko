// ast.cpp
#include "ast.hpp"
#include <stdexcept>
#include <cmath>

double Variable::evaluate(Context& ctx) const {
    auto it = ctx.variables.find(name);
    if (it == ctx.variables.end())
        throw std::runtime_error("Undefined variable: " + name);
    return it->second;
}

double Assignment::evaluate(Context& ctx) const {
    double val = value->evaluate(ctx);
    ctx.variables[name] = val;
    return val;
}


double FunctionCall::evaluate(Context& ctx) const {
    std::vector<double> argValues;
    for (const auto& arg : args) {
        argValues.push_back(arg->evaluate(ctx));
    }

    // Вбудовані функції
    if (name == "min") {
        if (argValues.size() != 2) throw std::runtime_error("min requires 2 arguments");
        return std::min(argValues[0], argValues[1]);
    }
    if (name == "max") {
        if (argValues.size() != 2) throw std::runtime_error("max requires 2 arguments");
        return std::max(argValues[0], argValues[1]);
    }
    if (name == "abs") {
        if (argValues.size() != 1) throw std::runtime_error("abs requires 1 argument");
        return std::abs(argValues[0]);
    }
    if (name == "pow") {
        if (argValues.size() != 2) throw std::runtime_error("pow requires 2 arguments");
        return std::pow(argValues[0], argValues[1]);
    }

    throw std::runtime_error("Unknown function: " + name);
}

double BinaryOp::evaluate(Context& ctx) const {
    double l = left->evaluate(ctx);
    double r = right->evaluate(ctx);

    switch (op) {
    case '+': return l + r;
    case '-': return l - r;
    case '*': return l * r;
    case '/':
        if (r == 0) throw std::runtime_error("Division by zero");
        return l / r;
    default: throw std::runtime_error("Unknown binary operator");
    }
}
