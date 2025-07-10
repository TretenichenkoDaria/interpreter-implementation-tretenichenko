#include "ast.hpp"
#include <stdexcept>
#include <cmath>

double Variable::evaluate(Context& ctx) const
{
    auto it = ctx.variables.find(name);
    if (it == ctx.variables.end())
    {
        throw runtime_error("undefined variable");
    }
    return it->second;
}

double Assignment::evaluate(Context& ctx) const
{
    double val = value->evaluate(ctx);
    ctx.variables[name] = val;
    return val;
}


double FunctionCall::evaluate(Context& ctx) const
{
    vector<double> argValues;
    for (const auto& arg : args)
    {
        argValues.push_back(arg->evaluate(ctx));
    }

    if (name == "min")
    {
        if (argValues.size() != 2)
        {
            throw runtime_error("min requires 2 arguments");
        }
        return min(argValues[0], argValues[1]);
    }

    if (name == "max")
    {
        if (argValues.size() != 2)
        {
            throw runtime_error("max requires 2 arguments");
        }
        return max(argValues[0], argValues[1]);
    }
    if (name == "abs")
    {
        if (argValues.size() != 1)
        {
            throw runtime_error("abs requires 1 argument");
        }
        return abs(argValues[0]);
    }
    if (name == "pow")
    {
        if (argValues.size() != 2)
        {
            throw runtime_error("pow requires 2 arguments");
        }
        return pow(argValues[0], argValues[1]);
    }

    throw runtime_error("unknown function");
}

double BinaryOp::evaluate(Context& ctx) const
{
    double l = left->evaluate(ctx);
    double r = right->evaluate(ctx);

    switch (operation)
    {
    case '+':
        return l + r;
    case '-':
        return l - r;
    case '*':
        return l * r;
    case '/':
        if (r == 0)
        {
            throw runtime_error("division by zero");
        }
        return l / r;

    default:
        throw runtime_error("unknown operator");
    }
}
