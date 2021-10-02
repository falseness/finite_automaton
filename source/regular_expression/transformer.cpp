#include "transformer.h"
#include <cassert>

RegularTransformer::RegularTransformer(const string& regular_expression) {
    result_ = make_unique<FiniteAutomaton>();
    regular_expression_ = "";
    for (auto c : regular_expression) {
        if (std::isspace(c) && c == '\0')
            continue;
        regular_expression_ += c;
    }
    CheckBracketsBalance();
    DeleteUselessAsteriskSymbols();
}

void RegularTransformer::Combine(
        SubAutomaton& sub_left, SubAutomaton& sub_right, const string& command) {
    if (command == "+")
        sub_left.Unite(sub_right);
    else if (command == "")
        sub_left.Concatenate(sub_right);
}


SubAutomaton RegularTransformer::ParseHere(
        string::iterator begin, string::iterator separator, string::iterator end) {
    auto left_sub_automaton = std::move(RecursiveParse(begin, separator));
    if ((*separator) == '*') {
        left_sub_automaton.Loop();
        if (++separator == end)
            return std::move(left_sub_automaton);
    }
    string command;
    if ((*separator) == '+') {
        command = "+";
        ++separator;
    }
    auto right_sub_automaton = std::move(RecursiveParse(separator, end));
    Combine(left_sub_automaton, right_sub_automaton, command);
    return std::move(left_sub_automaton);
}

SubAutomaton RegularTransformer::OrdinaryParse(string::iterator begin, string::iterator end) {
    for (auto it = begin; it != end; ++it) {
        char c = *it;
        if ('a' <= c && c <= 'z')
            continue;
        return ParseHere(begin, it, end);
    }
    string word;
    std::copy(begin, end, std::back_inserter(word));
    return {result_.get(), std::move(word)};

}

SubAutomaton RegularTransformer::BracketsParse(string::iterator begin, string::iterator end) {
    uint32_t brackets_depth = 0;
    auto it = begin;
    for (; it != end; ++it) {
        char c = *it;
        if (c == '(') {
            ++brackets_depth;
        }
        if (c != ')' || --brackets_depth) {
            continue;
        }
        if (++it == end) {
            return RecursiveParse(std::next(begin), std::prev(it));
        }
        return ParseHere(begin, it, end);
    }
    assert(false);
}

SubAutomaton RegularTransformer::RecursiveParse(string::iterator begin, string::iterator  end) {
    if (begin == end) {
        assert(false);
    }
    if ((*begin) == '(')
        return BracketsParse(begin, end);
    return OrdinaryParse(begin, end);
}

unique_ptr<FiniteAutomaton> RegularTransformer::Parse() {
    SubAutomaton sub_automaton = std::move(RecursiveParse(regular_expression_.begin(), regular_expression_.end()));
    result_->InitStartAndEndVertexes(std::move(sub_automaton));
    return std::move(result_);
}

void RegularTransformer::CheckBracketsBalance() {
    int depth = 0;
    for (auto c:regular_expression_) {
        if (c == '(') {
            ++depth;
            continue;
        }
        if (c == ')') {
            --depth;
            assert(depth >= 0);
        }
    }
    assert(!depth);
}

void RegularTransformer::DeleteUselessAsteriskSymbols() {
    string new_expression;
    for (auto it = regular_expression_.begin(); it != regular_expression_.end(); ++it) {
        new_expression += *it;
        if ((*it) != '*')
            continue;
        while (it != regular_expression_.end() && (*it) == '*') {
            ++it;
        }
        --it;
    }
    regular_expression_ = std::move(new_expression);
}
