#include "delete_long_words.h"
#include <cassert>

LongWordsDeleter::LongWordsDeleter(const FiniteAutomaton& automaton) :
    automaton_(automaton), graph_(automaton_.get_graph()), result_(automaton_.size(), automaton_.get_start()) {
    used_.resize(result_.size());
}

pair<FiniteAutomaton, vector<string>> LongWordsDeleter::Transform() {
    DFS(automaton_.get_start());
    vector<string> result_alphabet;
    for (auto some_char : alphabet_) {
        result_alphabet.emplace_back(string(1, some_char));
    }
    return {std::move(result_), std::move(result_alphabet)};
}

void LongWordsDeleter::DFS(FiniteAutomaton::Vertex v) {
    used_[v] = true;
    result_.set_final(v, automaton_.is_final(v));
    for (auto& edge : graph_[v]) {
        assert(!edge.word.empty());
        FiniteAutomaton::Vertex previous_vertex = v;
        for (size_t i = 0; i + 1 < edge.word.size(); ++i) {
            alphabet_.insert(edge.word[i]);

            FiniteAutomaton::Vertex new_vertex = result_.AddNewVertex();
            result_.AddNewEdge(previous_vertex, {new_vertex, string(1, edge.word[i])});
            previous_vertex = new_vertex;
        }
        alphabet_.insert(edge.word.back());
        result_.AddNewEdge(previous_vertex, {edge.finish, string(1, edge.word.back())});
        if (!used_[edge.finish])
            DFS(edge.finish);
    }
}
