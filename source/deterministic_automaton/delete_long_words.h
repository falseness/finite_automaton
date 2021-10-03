#include <source/sub_automaton.h>

#pragma once


class LongWordsDeleter {
    const FiniteAutomaton& automaton_;
    const vector<vector<FiniteAutomaton::Edge>>& graph_;
    FiniteAutomaton result_;
    set<char> alphabet_;
    vector<uint8_t> used_;
    void DFS(FiniteAutomaton::Vertex);
public:
    explicit LongWordsDeleter(const FiniteAutomaton&);
    // return automaton and sorted alphabet
    pair<FiniteAutomaton, vector<string>> Transform();
};