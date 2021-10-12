#include <source/automatons/deterministic.h>

#pragma once

class CompleteDeterministicAutomaton : public DeterministicAutomaton {
    vector<string> alphabet_;
    void InitializeAlphabet(const vector<vector<Edge>>&);
    static bool CompareEdges(const FiniteAutomaton::Edge&, const FiniteAutomaton::Edge&);
    static void DeleteSameVertexesEdges(vector<FiniteAutomaton::Edge>&);
public:
    CompleteDeterministicAutomaton(size_t, FiniteAutomaton::Vertex);
    explicit CompleteDeterministicAutomaton(const DeterministicAutomaton&);
    CompleteDeterministicAutomaton(const CompleteDeterministicAutomaton&) = default;
    CompleteDeterministicAutomaton(CompleteDeterministicAutomaton&&) = default;
    CompleteDeterministicAutomaton& operator=(CompleteDeterministicAutomaton&&) = default;
    [[nodiscard]] CompleteDeterministicAutomaton CreateMinimalDeterministicAutomaton() const;
    [[nodiscard]] string CreateRegularExpression() const;
    bool Contain(const string&);
    explicit CompleteDeterministicAutomaton(const string&);
    [[nodiscard]] CompleteDeterministicAutomaton CreateComplement() const;
    // both automatons should be minimal automatons
    bool IsIsomorphic(const CompleteDeterministicAutomaton&) const;
};
