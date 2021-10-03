#include <source/automatons/sub_automaton.h>

#pragma once

class RegularTransformer {
    string regular_expression_;
    FiniteAutomaton result_;
    void CheckBracketsBalance();
    void DeleteUselessAsteriskSymbols();

    void Combine(SubAutomaton&, SubAutomaton&, const string&);
    SubAutomaton ParseHere(string::iterator, string::iterator, string::iterator);
    SubAutomaton BracketsParse(string::iterator, string::iterator);
    SubAutomaton OrdinaryParse(string::iterator, string::iterator);
    SubAutomaton RecursiveParse(string::iterator, string::iterator);
public:
    explicit RegularTransformer(const string&);
    FiniteAutomaton Parse();
};