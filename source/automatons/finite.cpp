#include "finite.h"
#include <source/automatons/sub_automaton.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include <source/regular_expression/transformer.h>
#include <source/regular_expression/correct_string.h>

FiniteAutomaton::Vertex FiniteAutomaton::AddNewVertex() {
    graph_.emplace_back();
    return graph_.size() - 1;
}

void FiniteAutomaton::AddNewEdge(FiniteAutomaton::Vertex start, FiniteAutomaton::Edge&& edge) {
    graph_[start].push_back(std::move(edge));
}

void FiniteAutomaton::InitStartAndEndVertexes(SubAutomaton&& sub_automaton) {
    assert(!start_and_final_vertexes_.get());
    start_and_final_vertexes_ = make_unique<SubAutomaton>(std::move(sub_automaton));
}

void FiniteAutomaton::Output() const {
    size_t edges_count = 0;
    for (const auto& edges :graph_) {
        edges_count += edges.size();
    }
    std::cout << graph_.size() << ' ' << edges_count << '\n';
    for (size_t i = 0; i < graph_.size(); ++i) {
        for (auto& u : graph_[i]) {
            string word_output = u.word.empty() ? kOutputEmptyWord : u.word;
            std::cout << i << ' ' << u.finish << ' ' << word_output << '\n';
        }
    }
    //std::cout << "START\n";
    std::cout << start_and_final_vertexes_->start_ << '\n';
    //std::cout << "END\n";
    std::cout << start_and_final_vertexes_->final_vertexes_.size() << '\n';
    for (auto u : start_and_final_vertexes_->final_vertexes_) {
        std::cout << u << ' ';
    }
}

const vector<vector<FiniteAutomaton::Edge>>& FiniteAutomaton::get_graph() const {
    return graph_;
}

const SubAutomaton& FiniteAutomaton::get_start_and_end_vertexes() const {
    return *start_and_final_vertexes_;
}

FiniteAutomaton::FiniteAutomaton(size_t vertexes_count, Vertex start) :
        start_and_final_vertexes_(make_unique<SubAutomaton>(start)) {
    graph_.resize(vertexes_count);
}

size_t FiniteAutomaton::size() const {
    return graph_.size();
}

void FiniteAutomaton::set_final(FiniteAutomaton::Vertex vertex, bool should_be_final) {
    if (should_be_final)
        start_and_final_vertexes_->final_vertexes_.insert(vertex);
    else
        start_and_final_vertexes_->final_vertexes_.erase(vertex);
}

bool FiniteAutomaton::is_final(FiniteAutomaton::Vertex vertex) const {
    return start_and_final_vertexes_->final_vertexes_.contains(vertex);
}

void FiniteAutomaton::set_start(FiniteAutomaton::Vertex new_start) {
    start_and_final_vertexes_->start_ = new_start;
}

FiniteAutomaton::Vertex FiniteAutomaton::get_start() const {
    return start_and_final_vertexes_->start_;
}

void FiniteAutomaton::Input(std::ifstream& fin) {
    size_t count, edges_count;
    fin >> count >> edges_count;
    graph_.resize(count);
    for (size_t i = 0; i < edges_count; ++i) {
        FiniteAutomaton::Vertex a, b;
        string word;
        fin >> a >> b >> word;
        graph_[a].push_back({b, CorrectString(word, "")});
    }
    FiniteAutomaton::Vertex start_vertex;
    fin >> start_vertex;
    start_and_final_vertexes_ = make_unique<SubAutomaton>(start_vertex);
    size_t final_count;
    fin >> final_count;
    for (size_t i = 0; i < final_count; ++i) {
        FiniteAutomaton::Vertex final;
        fin >> final;
        set_final(final, true);
    }
}


string FiniteAutomaton::kEmptyWord = "`";
string FiniteAutomaton::kOutputEmptyWord = "ε";

void FiniteAutomaton::Clear() {
    FiniteAutomaton result;
    *this = std::move(result);
}

