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

void FiniteAutomaton::SetStartAndEndVertexes(SubAutomaton&& sub_automaton) {
    *start_and_end_vertexes_ = std::move(sub_automaton);
}

void FiniteAutomaton::Output() const {
    std::cout << graph_.size() << '\n';
    for (int i = 0; i < graph_.size(); ++i) {
        for (auto& u : graph_[i]) {
            string word_output = u.word == "" ? "ε" : u.word;
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

FiniteAutomaton::FiniteAutomaton() : start_and_end_vertexes_(make_unique<SubAutomaton>(this)) {}

