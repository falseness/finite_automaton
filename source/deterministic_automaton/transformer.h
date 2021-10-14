#include <source/automatons/deterministic.h>
#include <map>

#pragma once

class ToDeterministicTransformer {
    NoEmptyWordsAutomaton automaton_;
    DeterministicAutomaton result_;
    vector<uint8_t> used_;
    std::map<set<FiniteAutomaton::Vertex>, FiniteAutomaton::Vertex> vertex_tranformer_;
    vector<set<FiniteAutomaton::Vertex>> vertexes_stack_;

    vector<string> alphabet_;
public:
    explicit ToDeterministicTransformer(const NoEmptyWordsAutomaton&);
    DeterministicAutomaton Transform();
};