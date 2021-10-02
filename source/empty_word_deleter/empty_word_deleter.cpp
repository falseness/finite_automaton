#include "empty_word_deleter.h"

EmptyWordDeleter::EmptyWordDeleter(const FiniteAutomaton& automaton) :
        automaton_(automaton), graph_(automaton_.get_graph()), result_(automaton_.size(), automaton_.get_start()) {
    used_.resize(automaton.size());
}


FiniteAutomaton EmptyWordDeleter::DeleteEmptyWords() {
    for (FiniteAutomaton::Vertex i = 0; i < graph_.size(); ++i) {
        AddEdges(i);
    }
    return std::move(result_);
}

void EmptyWordDeleter::AddEdges(FiniteAutomaton::Vertex v) {
    vertex_edges_.clear();
    ++used_label_;
    is_final_ = false;

    DFS(v);

    sort(vertex_edges_.begin(), vertex_edges_.end(), EmptyWordDeleter::EdgeComparatorLess);

    auto unique_pos = std::unique(vertex_edges_.begin(), vertex_edges_.end(), EmptyWordDeleter::EdgeComparatorEqual);
    vertex_edges_.erase(unique_pos, vertex_edges_.end());
    for (auto edge : vertex_edges_) {
        result_.AddNewEdge(v, std::move(edge));
    }
    result_.set_final(v, is_final_);
}

void EmptyWordDeleter::DFS(FiniteAutomaton::Vertex v) {
    used_[v] = used_label_;
    if (automaton_.is_final(v)) {
        is_final_ = true;
    }
    for (const auto& edge : graph_[v]) {
        if (!edge.word.empty()) {
            vertex_edges_.push_back(edge);
            continue;
        }
        if (used_[edge.finish] == used_label_)
            continue;
        DFS(edge.finish);
    }
}

bool EmptyWordDeleter::EdgeComparatorLess(const FiniteAutomaton::Edge& edge1, const FiniteAutomaton::Edge& edge2) {
    if (edge1.finish < edge2.finish)
        return true;
    if (edge1.finish > edge2.finish)
        return false;
    return edge1.word < edge2.word;
}

bool EmptyWordDeleter::EdgeComparatorEqual(const FiniteAutomaton::Edge& edge1, const FiniteAutomaton::Edge& edge2) {
    return edge1.finish == edge2.finish && edge1.word == edge2.word;
}
