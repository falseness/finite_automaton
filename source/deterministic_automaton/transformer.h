#include <source/sub_automaton.h>
#include <map>

#pragma once

class ToDeterministicTransformer {
    FiniteAutomaton automaton_;
    FiniteAutomaton result_;
    vector<uint8_t> used_;
    std::map<set<FiniteAutomaton::Vertex>, FiniteAutomaton::Vertex> vertex_tranformer_;
    vector<set<FiniteAutomaton::Vertex>> vertexes_stack_;

    vector<string> alphabet_;
public:
    explicit ToDeterministicTransformer(const FiniteAutomaton&);
    FiniteAutomaton Transform();
};