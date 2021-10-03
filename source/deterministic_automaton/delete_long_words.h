#include <source/automatons/no_empty_words.h>

#pragma once


class LongWordsDeleter {
    const NoEmptyWordsAutomaton& automaton_;
    const vector<vector<FiniteAutomaton::Edge>>& graph_;
    NoEmptyWordsAutomaton result_;
    set<char> alphabet_;
    vector<uint8_t> used_;
    void DFS(FiniteAutomaton::Vertex);
public:
    explicit LongWordsDeleter(const NoEmptyWordsAutomaton&);
    // return automaton and sorted alphabet
    pair<NoEmptyWordsAutomaton, vector<string>> Transform();
};