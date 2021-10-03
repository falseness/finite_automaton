#include "complete_deterministic.h"
#include <cassert>

CompleteDeterministicAutomaton::CompleteDeterministicAutomaton(const DeterministicAutomaton& automaton) {
    SubAutomaton tmp(automaton.get_start_and_end_vertexes());
    InitStartAndEndVertexes(std::move(tmp));
    InitializeAlphabet(automaton.get_graph());

    graph_.resize(automaton.size());

    FiniteAutomaton::Vertex no_word_vertex = AddNewVertex();

    const vector<vector<FiniteAutomaton::Edge>>& old_graph = automaton.get_graph();

    for (FiniteAutomaton::Vertex i = 0; i < old_graph.size(); ++i) {
        graph_[i].resize(alphabet_.size());
        for (FiniteAutomaton::Vertex j = 0; j < graph_[i].size(); ++j) {
            graph_[i][j] = {no_word_vertex, alphabet_[j]};
        }
        for (const auto& edge : old_graph[i]) {
            auto it = std::lower_bound(alphabet_.begin(), alphabet_.end(), edge.word);
            assert(it != alphabet_.end() && it->size() == 1);
            graph_[i][it - alphabet_.begin()] = edge;
        }
    }
    graph_.back().resize(alphabet_.size());
    for (FiniteAutomaton::Vertex j = 0; j < graph_.back().size(); ++j) {
        graph_.back()[j] = {no_word_vertex, alphabet_[j]};
    }
}


DeterministicAutomaton CompleteDeterministicAutomaton::CreateMinimalDeterministicAutomaton() const {
    vector<vector<FiniteAutomaton::Vertex>> equivalence_classes;
    vector<FiniteAutomaton::Vertex> get_vertex_equivalence_class(graph_.size());

    equivalence_classes.resize(2);
    for (FiniteAutomaton::Vertex vertex = 0; vertex < graph_.size(); ++vertex) {
        equivalence_classes[is_final(vertex)].push_back(vertex);
        get_vertex_equivalence_class[vertex] = is_final(vertex);
    }

    bool was_changes = true;
    while (was_changes) {
        was_changes = false;
        vector<vector<FiniteAutomaton::Vertex>> new_equivalence_classes;
        for (auto& eq_class : equivalence_classes) {
            if (eq_class.size() == 1) {
                new_equivalence_classes.push_back(std::move(eq_class));
                continue;
            }
            vector<FiniteAutomaton::Vertex> new_classes[2];
            new_classes[0].push_back(eq_class[0]);
            for (size_t i = 1; i < eq_class.size(); ++i) {
                bool is_equal = true;
                for (int j = 0; j < graph_[eq_class[i]].size(); ++j) {
                    FiniteAutomaton::Vertex color1 =
                            get_vertex_equivalence_class[graph_[eq_class[0]][j].finish];
                    FiniteAutomaton::Vertex color2 =
                            get_vertex_equivalence_class[graph_[eq_class[i]][j].finish];
                    if (color1 != color2) {
                        is_equal = false;
                        break;
                    }
                }
                new_classes[!is_equal].push_back(eq_class[i]);
            }
            new_equivalence_classes.push_back(std::move(new_classes[0]));
            if (!new_classes[1].empty()) {
                was_changes = true;
                new_equivalence_classes.push_back(std::move(new_classes[1]));
            }
        }
        for (size_t i = 0; i < new_equivalence_classes.size(); ++i) {
            for (auto vertex : new_equivalence_classes[i]) {
                get_vertex_equivalence_class[vertex] = i;
            }
        }
        swap(new_equivalence_classes, equivalence_classes);
    }
    FiniteAutomaton::Vertex start_vertex;
    for (FiniteAutomaton::Vertex i = 0; i < equivalence_classes.size(); ++i) {
        for (auto vertex : equivalence_classes[i]) {
            if (get_start() == vertex)
                start_vertex = i;
        }
    }
    DeterministicAutomaton result(equivalence_classes.size(), start_vertex);
    for (FiniteAutomaton::Vertex i = 0; i < equivalence_classes.size(); ++i) {
        assert(!equivalence_classes[i].empty());
        FiniteAutomaton::Vertex some_element = equivalence_classes[i][0];
        bool is_i_final = is_final(some_element);

        for (const auto& edge : graph_[some_element]) {
            result.AddNewEdge(i, {get_vertex_equivalence_class[edge.finish], edge.word});
        }
        result.set_final(i, is_i_final);
    }

    return std::move(result);
}

void CompleteDeterministicAutomaton::InitializeAlphabet(const vector<vector<Edge>>& graph) {
    for (const auto& edges : graph) {
        for (const auto& edge : edges) {
            assert(edge.word.size() == 1);
            alphabet_.push_back(edge.word);
        }
    }
    sort(alphabet_.begin(), alphabet_.end());
    alphabet_.erase(std::unique(alphabet_.begin(), alphabet_.end()), alphabet_.end());
}
