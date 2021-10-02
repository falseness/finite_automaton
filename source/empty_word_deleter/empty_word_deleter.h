#include <source/sub_automaton.h>

#pragma once

class EmptyWordDeleter {
    const FiniteAutomaton& automaton_;
    const vector<vector<FiniteAutomaton::Edge>>& graph_;

    FiniteAutomaton result_;
    size_t used_label_ = 0;
    vector<size_t> used_;
    vector<FiniteAutomaton::Edge> vertex_edges_;
    bool is_final_;

    void AddEdges(FiniteAutomaton::Vertex);

    void DFS(FiniteAutomaton::Vertex);

    static bool EdgeComparatorLess(const FiniteAutomaton::Edge&, const FiniteAutomaton::Edge&);
    static bool EdgeComparatorEqual(const FiniteAutomaton::Edge&, const FiniteAutomaton::Edge&);
public:
    EmptyWordDeleter(const FiniteAutomaton&);
    FiniteAutomaton DeleteEmptyWords();
};