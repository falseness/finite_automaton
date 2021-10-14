#include "transformer.h"
#include <source/deterministic_automaton/delete_long_words.h>
#include <cassert>

ToDeterministicTransformer::ToDeterministicTransformer(const NoEmptyWordsAutomaton& automaton) {

    auto new_automaton_and_alphabet = LongWordsDeleter(automaton).Transform();
    automaton_ = std::move(new_automaton_and_alphabet.first);

    used_.resize(automaton.size());
    alphabet_ = std::move(new_automaton_and_alphabet.second);
}

DeterministicAutomaton ToDeterministicTransformer::Transform() {
    auto graph = automaton_.get_graph();
    set<FiniteAutomaton::Vertex> new_vertexes_set;

    new_vertexes_set.insert(automaton_.get_start());
    FiniteAutomaton::Vertex new_automaton_start = result_.AddNewVertex();
    result_.InitStartAndEndVertexes(SubAutomaton(new_automaton_start));
    vertex_tranformer_.emplace(new_vertexes_set, new_automaton_start);

    vertexes_stack_.push_back(std::move(new_vertexes_set));

    vector<set<FiniteAutomaton::Vertex>> new_edges(alphabet_.size());

    while (!vertexes_stack_.empty()) {
        for (auto& vertexes : new_edges) {
            vertexes.clear();
        }
        new_vertexes_set = std::move(vertexes_stack_.back());
        vertexes_stack_.pop_back();
        FiniteAutomaton::Vertex real_vertex_number = vertex_tranformer_[new_vertexes_set];
        bool is_final = false;
        for (FiniteAutomaton::Vertex vertex : new_vertexes_set) {
            is_final |= automaton_.is_final(vertex);
            for (auto& edge: graph[vertex]) {
                auto it = std::lower_bound(alphabet_.begin(), alphabet_.end(), edge.word);
                assert(it != alphabet_.end() && (*it) == edge.word);
                new_edges[it - alphabet_.begin()].insert(edge.finish);
            }
        }
        result_.set_final(real_vertex_number, is_final);

        for (size_t i = 0; i < new_edges.size(); ++i) {
            if (new_edges[i].empty())
                continue;
            auto it = vertex_tranformer_.find(new_edges[i]);
            if (it == vertex_tranformer_.end()) {
                FiniteAutomaton::Vertex new_vertex = result_.AddNewVertex();
                it = vertex_tranformer_.insert(make_pair(new_edges[i], new_vertex)).first;
                used_.push_back(false);
            }
            result_.AddNewEdge(real_vertex_number, {it->second, alphabet_[i]});
            if (!used_[it->second]) {
                used_[it->second] = true;
                vertexes_stack_.push_back(std::move(new_edges[i]));
            }
        }
    }
    return std::move(result_);
}
