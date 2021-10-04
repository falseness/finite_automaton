#include "sub_automaton.h"
#include <set>

SubAutomaton::SubAutomaton(FiniteAutomaton& automaton, string&& command){
    start_ = automaton.AddNewVertex();
    auto end = automaton.AddNewVertex();

    automaton.AddNewEdge(start_, {end, std::move(command)});
    final_vertexes_.insert(end);
}

SubAutomaton& SubAutomaton::Concatenate(FiniteAutomaton& automaton, SubAutomaton& sub_automaton) {
    for (FiniteAutomaton::Vertex end_vertex : final_vertexes_) {
        automaton.AddNewEdge(end_vertex, {sub_automaton.start_, ""});
    }
    final_vertexes_ = std::move(sub_automaton.final_vertexes_);

    return *this;
}

SubAutomaton& SubAutomaton::Unite(FiniteAutomaton& automaton, SubAutomaton& sub_automaton) {

    FiniteAutomaton::Vertex new_start = automaton.AddNewVertex();
    automaton.AddNewEdge(new_start, {start_, ""});
    automaton.AddNewEdge(new_start, {sub_automaton.start_, ""});
    final_vertexes_.merge(std::move(sub_automaton.final_vertexes_));

    start_ = new_start;
    return *this;
}

SubAutomaton& SubAutomaton::Loop(FiniteAutomaton& automaton) {
    FiniteAutomaton::Vertex new_start = automaton.AddNewVertex();
    automaton.AddNewEdge(new_start, {start_, ""});
    for (FiniteAutomaton::Vertex end_vertex: final_vertexes_) {
        automaton.AddNewEdge(end_vertex, {new_start, ""});
    }
    final_vertexes_.insert(new_start);

    start_ = new_start;
    return *this;
}

SubAutomaton::SubAutomaton(FiniteAutomaton::Vertex start_vertex) {
    start_ = start_vertex;
}
