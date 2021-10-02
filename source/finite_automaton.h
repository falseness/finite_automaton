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
    FiniteAutomaton(const FiniteAutomaton&) = delete;
    FiniteAutomaton(FiniteAutomaton&&) = default;
    void InitStartAndEndVertexes(SubAutomaton &&sub_automaton);
    void Output() const;
private:
    vector<vector<Edge>> graph_;
    unique_ptr<SubAutomaton> start_and_end_vertexes_;
public:
    FiniteAutomaton::Vertex AddNewVertex();
    void AddNewEdge(Vertex, Edge&&);
};
