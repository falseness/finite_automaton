# finite_automation

## Instructions: 
- compile - ./compile.sh
- run (after compilation) - ./run.sh
- run tests (after compilation) - ./test.sh

Input data file - input.txt

## Input format:
The first line contains a sequence of commands separated by |

in the next lines contain input of automaton/regular expression.

Commands are in text format.
The first command is an input of either automaton or regular expression.
There should not be more commands for input automaton or regular expression.
After the last command there should not be |

## What command means:
- `automaton` - reads finite automaton 
- `regular expression` - reads a regular expression 
- `reverse polish notation` - reads regular expression in reverse polish notation
- `to automaton` - converts the regular expression to automaton
- `delete empty words` - deletes empty words from a finite automaton
- `to dfa` - converts FA without empty words to a deterministic finite automaton
- `to complete dfa` - converts DFA to complete DFA
- `to minimal` - converts CDFA to minimal CDFA
- `to regular expression` - converts minimal CDFA to regular expression

As seen, for now, every previous command determines the next one.
For example, it means, that writing something like `automaton | to complete dfa` is prohibited, because before `complete dfa` should be preceded by `to dfa`   

## Format of input/output of regular expression
Allowed symbols: lowercase latin letters, symbol `ε` for empty word and symbols `()+*`

Kleene star applies to entire words. For example, `abba*` means `(abba)*`.
To apply Klenee star to part of a word, use brackets: `abb(a)*`

## Format of input of regular expression in reverse polish notation
`.` means concatenation. Then everything is the same.

For example, `ab.*c+` means `ab*+c`.

## Format of input/output of a finite automaton
The first line contains two numbers - the amount of vertices n and the amount of edges m. 
Next m lines contain edges - two indices beginning and end of edge and edge's word. 

Words should consist of lowercase latin letters.

The next line contains an index of starting vertex.

The next line contains the number k - the amount of final states. 
The next k lines contain indices of final vertices.

Vertices should have indices from 0 to n - 1 inclusively.

# More about input

There is an example of input in input.txt

If an input is not correct, then may appear Undefined Behaviour (but most likely the program will get assert exception).

There is another example of correct input:

regular expression | to automaton | delete empty words | to dfa | to complete dfa | to minimal | to regular expression

`aba+caba  *a+ε +b(a)*(a+b*)` 
