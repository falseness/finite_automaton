#include "finite_automaton.h"
#include <source/sub_automaton.h>
#include <cassert>
#include <iostream>

FiniteAutomaton::Vertex FiniteAutomaton::AddNewVertex() {
    graph_.emplace_back();
    return graph_.size() - 1;
}

void FiniteAutomaton::AddNewEdge(FiniteAutomaton::Vertex start, FiniteAutomaton::Edge&& edge) {
    graph_[start].push_back(std::move(edge));
}

void FiniteAutomaton::InitStartAndEndVertexes(SubAutomaton&& sub_automaton) {
    assert(!start_and_end_vertexes_.get());
    start_and_end_vertexes_ = make_unique<SubAutomaton>(std::move(sub_automaton));
}

void FiniteAutomaton::Output() const {
    static const string empty_word = "ε";
    std::cout << graph_.size() << '\n';
    for (int i = 0; i < graph_.size(); ++i) {
        for (auto& u : graph_[i]) {
            string word_output = u.word.empty() ? empty_word : u.word;
            std::cout << i << ' ' << u.finish << ' ' << word_output << '\n';
        }
    }
    std::cout << "START\n";
    std::cout << start_and_end_vertexes_->start_ << '\n';
    std::cout << "END\n";
    for (auto u:start_and_end_vertexes_->end_vertexes_) {
        std::cout << u << ' ';
    }
}

const vector<vector<FiniteAutomaton::Edge>>& FiniteAutomaton::get_graph() const {
    return graph_;
}

const SubAutomaton& FiniteAutomaton::get_start_and_end_vertexes() const {
    return *start_and_end_vertexes_;
}
