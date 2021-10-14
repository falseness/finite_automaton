#include <source/automatons/complete_deterministic.h>
#include <sstream>

#pragma once

class UserInterface {
    std::stringstream input_;

    string InputCommand();

    void InputAutomaton(std::ifstream&);
    void InputRegularExpression(std::ifstream&);
    void InputReversePolishNotationRegularExpression(std::ifstream&);

    void CommandAutomaton(FiniteAutomaton&&);
    void CommandRegularExpression(string&&);

    void DeleteEmptyWords(FiniteAutomaton&&);
    void ToDFA(NoEmptyWordsAutomaton&&);
    void ToCompleteDFA(DeterministicAutomaton&&);
    void ToMinimalDFA(CompleteDeterministicAutomaton&&);
    void ToRegularExpression(CompleteDeterministicAutomaton&&);
public:
    void Run();
    static string kInputAutomaton;
    static string kInputRegularExpression;
    static string kInputReversePolishNotationRegularExpression;
    static string kDeleteEmptyWords;
    static string kConvertToDFA;
    static string kConvertToCompleteDFA;
    static string kConvertToMinimalDFA;
    static string kConvertToRegularExpression;
    static string kConvertToAutomaton;
};
