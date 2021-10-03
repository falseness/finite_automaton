#include <source/automatons/sub_automaton.h>
#include <source/automatons/no_empty_words.h>

#pragma once

class EmptyWordDeleter {
    const FiniteAutomaton& automaton_;
    const vector<vector<FiniteAutomaton::Edge>>& graph_;

    NoEmptyWordsAutomaton result_;
    size_t used_label_ = 0;
    vector<size_t> used_;
    vector<FiniteAutomaton::Edge> vertex_edges_;
    bool is_final_;

    void AddEdges(FiniteAutomaton::Vertex);

    void DFS(FiniteAutomaton::Vertex);

    static bool EdgeComparatorLess(const FiniteAutomaton::Edge&, const FiniteAutomaton::Edge&);
    static bool EdgeComparatorEqual(const FiniteAutomaton::Edge&, const FiniteAutomaton::Edge&);
public:
    explicit EmptyWordDeleter(const FiniteAutomaton&);
    NoEmptyWordsAutomaton DeleteEmptyWords();
};