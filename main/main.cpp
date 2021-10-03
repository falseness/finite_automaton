#include <iostream>
#include <source/regular_expression/transformer.h>
#include <source/empty_word_deleter/empty_word_deleter.h>
#include <source/deterministic_automaton/transformer.h>

int main() {
    auto transformer = RegularTransformer("(a+b+c)*a");
    auto automaton = transformer.Parse();
    automaton.Output();
    std::cout << '\n';
    EmptyWordDeleter empty_word_deleter(automaton);
    auto new_automaton = empty_word_deleter.DeleteEmptyWords();
    new_automaton.Output();
    std::cout << '\n';
    auto d_automaton = ToDeterministicTransformer(new_automaton).Transform();
    d_automaton.Output();
    return 0;
}
