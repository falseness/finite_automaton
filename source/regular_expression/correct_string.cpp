#include "correct_string.h"

std::string CorrectString(const std::string& regular_expression, const std::string& empty_word) {
    std::string result;

    const char empty_symbol_start = -50;
    const char empty_symbol_end = -75;
    for (size_t i = 0; i < regular_expression.size(); ++i) {
        char c = regular_expression[i];
        if (std::isspace(c) || c == '\0')
            continue;
        if (i + 1 < regular_expression.size() &&
            c == empty_symbol_start && regular_expression[i + 1] == empty_symbol_end) {
            result += empty_word;
            ++i;
            continue;
        }
        result += c;
    }
    return result;
}