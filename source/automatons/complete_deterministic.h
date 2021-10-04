#include <source/automatons/deterministic.h>

#pragma once

class CompleteDeterministicAutomaton : public DeterministicAutomaton {
    vector<string> alphabet_;
    void InitializeAlphabet(const vector<vector<Edge>>&);
public:
    CompleteDeterministicAutomaton(size_t, FiniteAutomaton::Vertex);
    explicit CompleteDeterministicAutomaton(const DeterministicAutomaton&);
    [[nodiscard]] CompleteDeterministicAutomaton CreateMinimalDeterministicAutomaton() const;
    [[nodiscard]] string CreateRegularExpression() const;
    bool Contain(const string&);
};
