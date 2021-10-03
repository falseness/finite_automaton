#include <iostream>
#include <source/regular_expression/transformer.h>
#include <source/empty_word_deleter/empty_word_deleter.h>
#include <source/deterministic_automaton/transformer.h>
#include <source/automatons/complete_deterministic.h>

int main() {
    auto transformer = RegularTransformer("(a+b)*c");
    auto automaton = transformer.Parse();
    automaton.Output();
    std::cout << '\n';
    EmptyWordDeleter empty_word_deleter(automaton);
    auto new_automaton = empty_word_deleter.DeleteEmptyWords();
    new_automaton.Output();
    std::cout << '\n';
    auto d_automaton = ToDeterministicTransformer(new_automaton).Transform();
    d_automaton.Output();
    std::cout << '\n';
    CompleteDeterministicAutomaton complete_d(d_automaton);
    complete_d.Output();
    std::cout << '\n';
    auto min_d_automaton = complete_d.CreateMinimalDeterministicAutomaton();
    min_d_automaton.Output();
    return 0;
}
