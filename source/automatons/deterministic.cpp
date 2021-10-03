#include "deterministic.h"
#include <source/deterministic_automaton/transformer.h>
#include <cassert>

DeterministicAutomaton::DeterministicAutomaton(const NoEmptyWordsAutomaton& automaton) :
        DeterministicAutomaton(std::move(ToDeterministicTransformer(automaton).Transform())) {}
