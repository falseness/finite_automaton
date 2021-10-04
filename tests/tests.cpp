#include "tests.h"
#include <source/automatons/complete_deterministic.h>


void SomeTestCase::SetUp() {

}
void SomeTestCase::TearDown() {

}

TEST_F(SomeTestCase, SimpleTest) {
    CompleteDeterministicAutomaton automaton("a*b");
    EXPECT_FALSE(automaton.Contain("a"));
}

TEST_F(SomeTestCase, LongTest) {
    CompleteDeterministicAutomaton automaton("(a+b+c)*ab(a+c)");
    EXPECT_TRUE(automaton.Contain("abcaba"));
}

TEST_F(SomeTestCase, EmptyWordTest) {
    CompleteDeterministicAutomaton automaton("ε + a  (b)* + abaaaa");
    EXPECT_TRUE(automaton.Contain("") && automaton.Contain("a"));
}

TEST_F(SomeTestCase, FullCycleTest) {
    CompleteDeterministicAutomaton automaton("(aba + caba)*daba(dd+a)*");
    CompleteDeterministicAutomaton another(automaton.CreateRegularExpression());
    EXPECT_TRUE(another.Contain("abaabadabadddda") && another.Contain("daba") && !another.Contain("abadab"));
}

TEST_F(SomeTestCase, TestMinimalAutomaton) {
    CompleteDeterministicAutomaton automaton("(a+b)*");
    CompleteDeterministicAutomaton minimal_automaton = automaton.CreateMinimalDeterministicAutomaton();
    EXPECT_TRUE(minimal_automaton.size() == 1 && minimal_automaton.Contain("ababaababbaabbababba"));
}
