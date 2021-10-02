#include <source/finite_automaton.h>

#pragma once

class SubAutomaton {
public:
    FiniteAutomaton::Vertex start_;
    set<FiniteAutomaton::Vertex> final_vertexes_;

    SubAutomaton(FiniteAutomaton&, string&&);
    explicit SubAutomaton(FiniteAutomaton::Vertex);
    SubAutomaton(const SubAutomaton&) = delete;
    SubAutomaton(SubAutomaton&&) = default;
    SubAutomaton& operator=(SubAutomaton&&) = default;
    SubAutomaton& Concatenate(FiniteAutomaton&, SubAutomaton&);
    SubAutomaton& Unite(FiniteAutomaton&, SubAutomaton&);
    SubAutomaton& Loop(FiniteAutomaton&);
};
