#include "sub_automaton.h"

SubAutomaton::SubAutomaton(FiniteAutomaton* automaton, string&& command) : automaton_(automaton) {
    start_ = automaton_->AddNewVertex();
    auto end = automaton_->AddNewVertex();

    automaton_->AddNewEdge(start_, {end, std::move(command)});
    end_vertexes_.insert(end);
}

SubAutomaton& SubAutomaton::Concatenate(SubAutomaton& sub_automaton) {
    for (FiniteAutomaton::Vertex end_vertex : end_vertexes_) {
        automaton_->AddNewEdge(end_vertex, {sub_automaton.start_, ""});
    }
    end_vertexes_ = std::move(sub_automaton.end_vertexes_);

    return *this;
}

SubAutomaton& SubAutomaton::Unite(SubAutomaton& sub_automaton) {

    FiniteAutomaton::Vertex new_start = automaton_->AddNewVertex();
    automaton_->AddNewEdge(new_start, {start_, ""});
    automaton_->AddNewEdge(new_start, {sub_automaton.start_, ""});
    end_vertexes_.merge(std::move(sub_automaton.end_vertexes_));

    start_ = new_start;
    return *this;
}

SubAutomaton& SubAutomaton::Loop() {
    FiniteAutomaton::Vertex new_start = automaton_->AddNewVertex();
    automaton_->AddNewEdge(new_start, {start_, ""});
    for (FiniteAutomaton::Vertex end_vertex: end_vertexes_) {
        automaton_->AddNewEdge(end_vertex, {new_start, ""});
    }
    end_vertexes_.insert(new_start);

    start_ = new_start;
    return *this;
}

SubAutomaton::SubAutomaton(FiniteAutomaton* automaton) {
    automaton_ = automaton;
}
