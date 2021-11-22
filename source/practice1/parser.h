#include <set>

#pragma once


// уже есть функция ReversePolishNotationParser в source/regular_expression_parse_reverse_polish_notation.h
// но у нее должна быть принципиально другая функциональность, поэтому создаю новый класс.
class ReversePolishNotationParser {
    static const int8_t kErrorReturn = -1;
    static bool IsCorrectChar(char c);
    static std::string DeleteSpases(const std::string&);
public:
    static int8_t Parse(const std::string&, size_t k, size_t l);
};

class RegularExpressionParameters {
    std::set<size_t> achievable_lengths_;
    static size_t CalculateGreatestCommonFactor(size_t, size_t);
public:
    explicit RegularExpressionParameters(char c);
    void Concatenate(const RegularExpressionParameters&, size_t);
    void Unite(RegularExpressionParameters&, size_t);
    void Closure(size_t);
    bool is_achievable(size_t);
    bool has_achievable_lengths();
};
