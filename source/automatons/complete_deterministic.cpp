#include "complete_deterministic.h"
#include <cassert>
#include <source/regular_expression/transformer.h>

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


CompleteDeterministicAutomaton CompleteDeterministicAutomaton::CreateMinimalDeterministicAutomaton() const {
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
    CompleteDeterministicAutomaton result(equivalence_classes.size(), start_vertex);
    for (FiniteAutomaton::Vertex i = 0; i < equivalence_classes.size(); ++i) {
        assert(!equivalence_classes[i].empty());
        FiniteAutomaton::Vertex some_element = equivalence_classes[i][0];
        bool is_i_final = is_final(some_element);

        for (const auto& edge : graph_[some_element]) {
            result.AddNewEdge(i, {get_vertex_equivalence_class[edge.finish], edge.word});
        }
        result.set_final(i, is_i_final);
    }

    result.DeleteNoPathVertexes();
    result.InitializeAlphabet(result.get_graph());
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

string CompleteDeterministicAutomaton::CreateRegularExpression() const {
    const auto& tmp_graph = get_graph();

    vector<vector<Edge>> graph(tmp_graph.size() + 1);
    graph[0].push_back({(FiniteAutomaton::Vertex)(get_start() + 1), ""});

    FiniteAutomaton::Vertex final_vertex = graph.size();

    for (int i = 1; i < graph.size(); ++i) {
        for (const auto& edge : tmp_graph[i - 1]) {
            graph[i].push_back({(FiniteAutomaton::Vertex)(edge.finish + 1), edge.word});
        }
        if (is_final(i - 1))
            graph[i].push_back({final_vertex, ""});
    }
    assert(!graph.empty());
    while (graph.size() != 1) {
        vector<Edge> out_vertexes;
        vector<Edge> cycle_edges;

        FiniteAutomaton::Vertex deleting_vertex = graph.size() - 1;
        for (auto& edge : graph.back()) {
            if (edge.finish == deleting_vertex) {
                cycle_edges.push_back(std::move(edge));
            }
            else
                out_vertexes.push_back(std::move(edge));
        }
        string cycles_regular;
        DeleteSameVertexesEdges(cycle_edges);
        if (!cycle_edges.empty()) {
            assert(cycle_edges.size() == 1);
            cycles_regular = "(";
            cycles_regular += cycle_edges.back().word;
            cycles_regular += ")*";
        }
        DeleteSameVertexesEdges(out_vertexes);

        graph.pop_back();
        for (FiniteAutomaton::Vertex i = 0; i < graph.size(); ++i) {
            vector<FiniteAutomaton::Edge> new_edges;
            vector<FiniteAutomaton::Edge> edges_to_deleted_vertex;
            for (auto& old_edge : graph[i]) {
                if (old_edge.finish != deleting_vertex) {
                    new_edges.push_back(std::move(old_edge));
                    continue;
                }
                edges_to_deleted_vertex.push_back(old_edge);
            }
            DeleteSameVertexesEdges(edges_to_deleted_vertex);
            if (!edges_to_deleted_vertex.empty()) {
                assert(edges_to_deleted_vertex.size() == 1);
                const auto& old_edge = edges_to_deleted_vertex.back();
                for (const auto &out_edge: out_vertexes) {
                    new_edges.push_back({out_edge.finish, old_edge.word + cycles_regular + out_edge.word});
                }
            }
            swap(new_edges, graph[i]);
        }
    }

    string cycles_regular;
    string main_regular;
    vector<FiniteAutomaton::Edge> cycle_edges;
    vector<FiniteAutomaton::Edge> main_edges;

    for (auto& edge : graph[0]) {
        if (edge.finish) {
            main_edges.emplace_back(std::move(edge));
        }
        else {
            cycle_edges.emplace_back(std::move(edge));
        }
    }
    DeleteSameVertexesEdges(main_edges);
    DeleteSameVertexesEdges(cycle_edges);
    if (!cycle_edges.empty()) {
        cycles_regular = std::move(cycle_edges.back().word);
        cycles_regular += "*";
    }
    if (!main_edges.empty()) {
        main_regular = std::move(main_edges.back().word);
    }

    return cycles_regular + main_regular;
}

CompleteDeterministicAutomaton::CompleteDeterministicAutomaton(size_t count, FiniteAutomaton::Vertex start) :
    DeterministicAutomaton(count, start) {}

bool CompleteDeterministicAutomaton::Contain(const string& word) {
    for (const auto& c : word) {
        auto it = std::lower_bound(alphabet_.begin(), alphabet_.end(), string(1, c));
        if (it == alphabet_.end() || (*it) != string(1, c))
            return false;
    }
    FiniteAutomaton::Vertex pos = get_start();
    for (const auto& c : word) {
        auto it = std::lower_bound(alphabet_.begin(), alphabet_.end(), string(1, c));
        const auto& edge = graph_[pos][it - alphabet_.begin()];
        assert(edge.word == string(1, c));
        pos = edge.finish;
    }
    return is_final(pos);
}

CompleteDeterministicAutomaton::CompleteDeterministicAutomaton(const string& regular_expression) :
    CompleteDeterministicAutomaton(DeterministicAutomaton(NoEmptyWordsAutomaton(
            RegularTransformer(regular_expression).Parse()))) {}

CompleteDeterministicAutomaton CompleteDeterministicAutomaton::CreateComplement() const {
    CompleteDeterministicAutomaton result(*this);
    for (FiniteAutomaton::Vertex vertex = 0; vertex < graph_.size(); ++vertex) {
        result.set_final(vertex, !is_final(vertex));
    }
    return std::move(result);
}

bool CompleteDeterministicAutomaton::IsIsomorphic(const CompleteDeterministicAutomaton& automaton) const {
    if (size() != automaton.size())
        return false;
    if (alphabet_ != automaton.alphabet_)
        return false;

    vector<FiniteAutomaton::Vertex> isomorphism(size(), size());
    vector<FiniteAutomaton::Vertex> dfs_stack;
    isomorphism[get_start()] = automaton.get_start();
    dfs_stack.push_back(get_start());

    while (!dfs_stack.empty()) {
        FiniteAutomaton::Vertex vertex = dfs_stack.back();
        dfs_stack.pop_back();
        for (size_t i = 0; i < graph_[vertex].size(); ++i) {
            FiniteAutomaton::Vertex another_vertex = graph_[vertex][i].finish;
            FiniteAutomaton::Vertex isomorphic_vertex = automaton.graph_[isomorphism[vertex]][i].finish;
            if (isomorphism[another_vertex] == size()) {
                isomorphism[another_vertex] = isomorphic_vertex;
                dfs_stack.push_back(another_vertex);
            }
            else if (isomorphism[another_vertex] != isomorphic_vertex){
                return false;
            }
        }
    }
    for (FiniteAutomaton::Vertex i = 0; i < isomorphism.size(); ++i) {
        assert(isomorphism[i] != size());
        if (is_final(i) != automaton.is_final(isomorphism[i]))
            return false;
    }
    return true;
}

bool CompleteDeterministicAutomaton::CompareEdges(
        const FiniteAutomaton::Edge& edge1, const FiniteAutomaton::Edge& edge2) {
    return edge1.finish < edge2.finish;
}

void CompleteDeterministicAutomaton::DeleteSameVertexesEdges(vector<FiniteAutomaton::Edge>& out_vertexes) {
    sort(out_vertexes.begin(), out_vertexes.end(), CompareEdges);
    vector<Edge> deleted_same_vertexes;
    for (size_t i = 0; i < out_vertexes.size(); ++i) {
        string regular_expression;
        FiniteAutomaton::Vertex vertex = out_vertexes[i].finish;
        bool was_empty_word = false;
        size_t count_vertexes = 0;
        while (i < out_vertexes.size() && out_vertexes[i].finish == vertex) {
            if (out_vertexes[i].word.empty()) {
                was_empty_word = true;
            }
            else if (!regular_expression.empty())
                regular_expression += "+" + out_vertexes[i].word;
            else
                regular_expression += "(" + out_vertexes[i].word;
            ++i;
            ++count_vertexes;
        }
        if (!regular_expression.empty() && was_empty_word) {
            regular_expression += "+" + FiniteAutomaton::kEmptyWord;
        }
        if (!regular_expression.empty())
            regular_expression += ")";
        --i;
        if (count_vertexes == 1)
            regular_expression = out_vertexes[i].word;

        deleted_same_vertexes.push_back({vertex, regular_expression});
    }
    out_vertexes = std::move(deleted_same_vertexes);
}
