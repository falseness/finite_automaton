#include <vector>
#include <algorithm>
#include "parser.h"

int8_t ReversePolishNotationParser::Parse(const std::string& input_expression, size_t k, size_t l) {
    std::string regular_expression = std::move(DeleteSpases(input_expression));

    for (auto c : regular_expression) {
        if (!IsCorrectChar(c))
            return kErrorReturn;
    }
    std::vector<RegularExpressionParameters> stack;
    for (auto c : regular_expression) {
        if ('a' <= c && c <= 'z' || c == '0' || c == '1') {
            stack.emplace_back(c);
            continue;
        }
        if (c == '*') {
            if (stack.empty())
                return kErrorReturn;
            stack.back().Closure(k);
            continue;
        }
        if (stack.size() < 2)
            return kErrorReturn;
        if (c == '.')
            stack[stack.size() - 2].Concatenate(stack.back(), k);
        else
            stack[stack.size() - 2].Unite(stack.back(), k);
        stack.pop_back();
    }
    if (stack.size() != 1)
        return kErrorReturn;

    bool result;
    if (k == 1)
        result = stack.back().has_achievable_lengths();
    else
        result = stack.back().is_achievable(l);
    return static_cast<int8_t>(result);
}

bool ReversePolishNotationParser::IsCorrectChar(char c) {
    if ('a' <= c && c <= 'z')
        return true;
    static const std::vector<char> correct_not_alpha = {'0', '1', '.', '*', '+'};
    return std::find(correct_not_alpha.begin(), correct_not_alpha.end(), c) != correct_not_alpha.end();
}

std::string ReversePolishNotationParser::DeleteSpases(const std::string& s) {
    std::string result;
    for (char c : s) {
        if (std::isspace(c) || c == '\0')
            continue;
        result += c;
    }
    return std::move(result);
}

RegularExpressionParameters::RegularExpressionParameters(char c) {
    if (c == '0')
        return;
    if (c == '1')
        achievable_lengths_.insert(0);
    else
        achievable_lengths_.insert(1);
}

bool RegularExpressionParameters::has_achievable_lengths() {
    return !achievable_lengths_.empty();
}

bool RegularExpressionParameters::is_achievable(size_t len) {
    return achievable_lengths_.find(len) != achievable_lengths_.end();
}

void RegularExpressionParameters::Concatenate(const RegularExpressionParameters& other, size_t module) {
    std::set<size_t> result;
    for (size_t len : achievable_lengths_) {
        for (size_t other_len : other.achievable_lengths_) {
            result.insert((len + other_len) % module);
        }
    }
    achievable_lengths_.swap(result);
}

void RegularExpressionParameters::Unite(RegularExpressionParameters& other, size_t) {
    achievable_lengths_.merge(other.achievable_lengths_);
}

void RegularExpressionParameters::Closure(size_t module) {
    size_t greater_common_factor = 0;
    for (size_t len : achievable_lengths_) {
        greater_common_factor = CalculateGreatestCommonFactor(greater_common_factor, len);
    }
    std::set<size_t> result;
    for (size_t achievable_len = greater_common_factor; achievable_len;
            achievable_len = (achievable_len + greater_common_factor) % module) {
        result.insert(achievable_len);
    }
    result.insert(0);
    achievable_lengths_.swap(result);
}

size_t RegularExpressionParameters::CalculateGreatestCommonFactor(size_t a, size_t b) {
    if (!b)
        return a;
    return CalculateGreatestCommonFactor(b, a % b);
}
