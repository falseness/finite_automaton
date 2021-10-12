#include "parse_reverse_polish_notation.h"

std::string ParseRegularExpressionInReversePolishNotation(const std::string& polish_notation) {
    std::vector<std::string> regular_expressions_stack;
    for (auto u : polish_notation) {
        if (u == '*') {
            assert(!regular_expressions_stack.empty());
            std::string expression = std::move(regular_expressions_stack.back());
            regular_expressions_stack.pop_back();
            expression += "*";
            regular_expressions_stack.push_back(expression);
            continue;
        }
        if (u == '+' || u == '.') {
            std::string expression_operation = (u == '.') ? "" : "+";

            assert(regular_expressions_stack.size() >= 2);
            std::string right_expression = std::move(regular_expressions_stack.back());
            regular_expressions_stack.pop_back();
            std::string left_expression = std::move(regular_expressions_stack.back());
            regular_expressions_stack.pop_back();

            left_expression += expression_operation;
            left_expression += right_expression;
            left_expression += ")";
            regular_expressions_stack.push_back("(" + left_expression);
            continue;
        }
        regular_expressions_stack.emplace_back(1, u);
    }
    assert(regular_expressions_stack.size() == 1);
    return std::move(regular_expressions_stack.back());
}
