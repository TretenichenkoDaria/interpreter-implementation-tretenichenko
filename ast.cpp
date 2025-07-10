#include "ast.hpp"
#include <stdexcept>
#include <cmath>

double Variable::evaluate(Context& context)
{
    auto iterator = context.variables.find(name);
    if (iterator == context.variables.end())
    {
        throw runtime_error("undefined variable");
    }
    return iterator->second;
}

double Assignment::evaluate(Context& context)
{
    double val = value->evaluate(context);
    context.variables[name] = val;
    return val;
}


double FunctionCall::evaluate(Context& context)
{
    vector<double> argValues;
    for (const auto& arg : args)
    {
        argValues.push_back(arg->evaluate(context));
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

double BinaryOperation::evaluate(Context& context)
{
    double l = left->evaluate(context);
    double r = right->evaluate(context);

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
