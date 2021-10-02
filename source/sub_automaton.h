#include <source/finite_automaton.h>

#pragma once

class SubAutomaton {
public:
    FiniteAutomaton* automaton_;
public:
    FiniteAutomaton::Vertex start_;
    set<FiniteAutomaton::Vertex> end_vertexes_;

    SubAutomaton(FiniteAutomaton*, string&&);
    SubAutomaton(const SubAutomaton&) = delete;
    SubAutomaton(SubAutomaton&&) = default;
    SubAutomaton& operator=(SubAutomaton&&) = default;
    SubAutomaton& Concatenate(SubAutomaton&);
    SubAutomaton& Unite(SubAutomaton&);
    SubAutomaton& Loop();
};
