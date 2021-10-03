#include <source/automatons/finite.h>

#pragma once

class SubAutomaton {
public:
    FiniteAutomaton::Vertex start_;
    set<FiniteAutomaton::Vertex> final_vertexes_;

    SubAutomaton(FiniteAutomaton&, string&&);
    explicit SubAutomaton(FiniteAutomaton::Vertex);
    explicit SubAutomaton(const SubAutomaton&) = default;
    SubAutomaton(SubAutomaton&&) = default;
    SubAutomaton& operator=(SubAutomaton&&) = default;
    SubAutomaton& Concatenate(FiniteAutomaton&, SubAutomaton&);
    SubAutomaton& Unite(FiniteAutomaton&, SubAutomaton&);
    SubAutomaton& Loop(FiniteAutomaton&);
};
