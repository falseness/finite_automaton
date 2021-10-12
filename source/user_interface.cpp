#include "user_interface.h"
#include <fstream>
#include <iostream>
#include <cassert>
#include <source/regular_expression/transformer.h>
#include <source/regular_expression/parse_reverse_polish_notation.h>

void UserInterface::Run() {
    std::ifstream fin("input.txt");

    string commands;
    getline(fin, commands);
    input_ << commands;

    string input_command = InputCommand();
    if (input_command == kInputAutomaton)
        InputAutomaton(fin);
    else if (input_command == kInputRegularExpression)
        InputRegularExpression(fin);
    else if (input_command == kInputReversePolishNotationRegularExpression) {
        InputReversePolishNotationRegularExpression(fin);
    }
    else
        assert(false);
}

void UserInterface::InputAutomaton(std::ifstream& fin) {
    FiniteAutomaton automaton;
    automaton.Input(fin);

    CommandAutomaton(std::move(automaton));
}

void UserInterface::InputRegularExpression(std::ifstream& fin) {
    string result;
    getline(fin, result);
    CommandRegularExpression(std::move(result));
}

void UserInterface::InputReversePolishNotationRegularExpression(std::ifstream& fin) {
    string result;
    getline(fin, result);
    CommandRegularExpression(std::move(ParseRegularExpressionInReversePolishNotation(result)));
}

void UserInterface::CommandAutomaton(FiniteAutomaton&& automaton) {
    string command = InputCommand();
    if (command.empty()) {
        automaton.Output();
        return;
    }
    assert(command == UserInterface::kDeleteEmptyWords);
    DeleteEmptyWords(std::move(automaton));
}

void UserInterface::CommandRegularExpression(string&& regular_expression) {
    string command = InputCommand();
    if (command.empty()) {
        RegularTransformer::OutputExpression(regular_expression);
        return;
    }
    assert(command == UserInterface::kConvertToAutomaton);
    FiniteAutomaton automaton = RegularTransformer(regular_expression).Parse();
    CommandAutomaton(std::move(automaton));
}

void UserInterface::DeleteEmptyWords(FiniteAutomaton&& automaton) {
    NoEmptyWordsAutomaton new_automaton(automaton);
    automaton.Clear();
    string command = InputCommand();
    if (command.empty()) {
        new_automaton.Output();
        return;
    }

    assert(command == UserInterface::kConvertToDFA);
    ToDFA(std::move(new_automaton));
}

void UserInterface::ToDFA(NoEmptyWordsAutomaton&& automaton) {
    DeterministicAutomaton new_automaton(automaton);
    string command = InputCommand();
    if (command.empty()) {
        new_automaton.Output();
        return;
    }
    assert(command == UserInterface::kConvertToCompleteDFA);
    ToCompleteDFA(std::move(new_automaton));
}

void UserInterface::ToCompleteDFA(DeterministicAutomaton&& automaton) {
    CompleteDeterministicAutomaton new_automaton(automaton);
    automaton.Clear();
    string command = InputCommand();
    if (command.empty()) {
        new_automaton.Output();
        return;
    }
    assert(command == UserInterface::kConvertToMinimalDFA);
    ToMinimalDFA(std::move(new_automaton));
}

void UserInterface::ToMinimalDFA(CompleteDeterministicAutomaton&& automaton) {
    CompleteDeterministicAutomaton new_automaton = automaton.CreateMinimalDeterministicAutomaton();
    automaton.Clear();
    string command = InputCommand();
    if (command.empty()) {
        new_automaton.Output();
        return;
    }
    assert(command == UserInterface::kConvertToRegularExpression);
    ToRegularExpression(std::move(new_automaton));
}

void UserInterface::ToRegularExpression(CompleteDeterministicAutomaton&& automaton) {
    string regular_expression = automaton.CreateRegularExpression();
    automaton.Clear();
    UserInterface::CommandRegularExpression(std::move(regular_expression));
}

string UserInterface::InputCommand() {
    string cmd;
    string next_command;
    while (input_ >> cmd) {
        if (cmd == "|")
            break;
        if (!next_command.empty())
            next_command += " ";
        next_command += cmd;
    }
    return next_command;
}

string UserInterface::kInputAutomaton = "automaton";
string UserInterface::kInputRegularExpression = "regular expression";
string UserInterface::kInputReversePolishNotationRegularExpression = "reverse polish notation";


string UserInterface::kDeleteEmptyWords = "delete empty words";
string UserInterface::kConvertToDFA = "to dfa";
string UserInterface::kConvertToCompleteDFA = "to complete dfa";
string UserInterface::kConvertToMinimalDFA = "to minimal";
string UserInterface::kConvertToRegularExpression = "to regular expression";
string UserInterface::kConvertToAutomaton = "to automaton";
