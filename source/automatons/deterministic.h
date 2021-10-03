#include <source/automatons/no_empty_words.h>

#pragma once


class DeterministicAutomaton : public NoEmptyWordsAutomaton {
public:
    using NoEmptyWordsAutomaton::NoEmptyWordsAutomaton;
    explicit DeterministicAutomaton(const NoEmptyWordsAutomaton&);
};
