#include <vector>
#include <string>
#include <set>
#include <memory>

class SubAutomaton;

#pragma once

using std::vector;
using std::pair;
using std::make_pair;
using std::string;
using std::unique_ptr;
using std::make_unique;
using std::set;

class FiniteAutomaton {
public:
    typedef uint16_t Vertex;
    struct Edge {
        Vertex finish;
        string word;
    };
    FiniteAutomaton() = default;
    FiniteAutomaton(const FiniteAutomaton&);
    FiniteAutomaton(FiniteAutomaton&&) = default;
    FiniteAutomaton& operator=(FiniteAutomaton&&) = default;
    FiniteAutomaton(size_t, Vertex);
    void InitStartAndEndVertexes(SubAutomaton&& sub_automaton);
    void Output() const;
    void Input(std::ifstream&);
protected:
    void DeleteNoPathVertexes();
    void DFS(FiniteAutomaton::Vertex, vector<uint8_t>&, FiniteAutomaton&, vector<FiniteAutomaton::Vertex>&) const;
protected:
    vector<vector<Edge>> graph_;
    unique_ptr<SubAutomaton> start_and_final_vertexes_;
public:
    static string kEmptyWord;
    static string kOutputEmptyWord;
    [[nodiscard]] size_t size() const;
    [[nodiscard]] const vector<vector<Edge>>& get_graph() const;
    [[nodiscard]] const SubAutomaton& get_start_and_end_vertexes() const;
    FiniteAutomaton::Vertex AddNewVertex();
    void set_final(Vertex, bool);
    [[nodiscard]] bool is_final(Vertex vertex) const;
    void set_start(Vertex);
    [[nodiscard]] Vertex get_start() const;
    void AddNewEdge(Vertex, Edge&&);
    void Clear();
};
