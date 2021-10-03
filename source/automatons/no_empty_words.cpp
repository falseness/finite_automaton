#include "no_empty_words.h"
#include <source/empty_word_deleter/empty_word_deleter.h>

NoEmptyWordsAutomaton::NoEmptyWordsAutomaton(const FiniteAutomaton& automaton) :
        NoEmptyWordsAutomaton(std::move(EmptyWordDeleter(automaton).DeleteEmptyWords())) {}
