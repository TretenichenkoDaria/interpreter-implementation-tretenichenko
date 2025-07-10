#include <iostream>
#include <string>

#include "ast.hpp"
using namespace std;

ExprPtr parse(const std::string& input);

int main()
{
    Context ctx;

    string line;
    cout << "> ";
    while (getline(cin, line))
    {
        if (line.empty())
        {
            cout << "> ";
            continue;
        }

        try
        {
            auto expr = parse(line);
            double result = expr->evaluate(ctx);
            cout << result << endl;
        }
        catch (const exception& ex)
        {
            cerr << "error: " << ex.what() << endl;
        }

        cout << "> ";
    }

    return 0;
}
