#include <source/automatons/sub_automaton.h>

#pragma once

class NoEmptyWordsAutomaton : public FiniteAutomaton {
public:
    using FiniteAutomaton::FiniteAutomaton;
    explicit NoEmptyWordsAutomaton(const FiniteAutomaton&);
};
