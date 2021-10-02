#include <iostream>
#include <source/regular_expression/transformer.h>
#include <source/empty_word_deleter/empty_word_deleter.h>

int main() {
    auto transformer = RegularTransformer("a*");
    auto automaton = transformer.Parse();
    automaton.Output();
    std::cout << '\n';
    EmptyWordDeleter empty_word_deleter(automaton);
    auto new_automaton = empty_word_deleter.DeleteEmptyWords();
    new_automaton.Output();
    return 0;
}
