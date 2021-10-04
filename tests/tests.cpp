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

TEST_F(SomeTestCase, NoLettertest) {
    CompleteDeterministicAutomaton automaton("(a+b)*");
    EXPECT_FALSE(automaton.Contain("y"));
}

TEST_F(SomeTestCase, CompleteAutomatonGraph) {
    CompleteDeterministicAutomaton automaton("abc+(ca*)+b*a*(caacaca)");
    const size_t chars_cnt = 3;
    const auto& graph = automaton.get_graph();
    EXPECT_FALSE(graph.empty());
    for (const auto& edges : graph) {
        EXPECT_TRUE(edges.size() == chars_cnt);
        for (size_t i = 0; i < edges.size(); ++i) {
            EXPECT_TRUE(edges[i].word == string(1, 'a' + i));
        }
    }
}

TEST_F(SomeTestCase, ThreeAutomatonsThreeLettersCheck) {
    CompleteDeterministicAutomaton automaton("(ab(c)*+da)+hi+(ab)*(c*d)*+fef+(da)");
    CompleteDeterministicAutomaton minimal_automaton = automaton.CreateMinimalDeterministicAutomaton();
    CompleteDeterministicAutomaton another(minimal_automaton.CreateRegularExpression());

    for (char c1 = 'a'; c1 <= 'z'; ++c1) {
        for (char c2 = 'a'; c2 <= 'z'; ++c2) {
            for (char c3 = 'a'; c3 <= 'z'; ++c3) {
                string regular_expression;
                regular_expression += c1;
                regular_expression += c2;
                regular_expression += c3;
                bool b1 = automaton.Contain(regular_expression);
                bool b2 = minimal_automaton.Contain(regular_expression);
                bool b3 = another.Contain(regular_expression);
                EXPECT_TRUE(b1 == b2 && b2 == b3);
            }
        }
    }
}

bool get_bit(size_t mask, size_t i) {
    return (mask >> i) & 1;
}

TEST_F(SomeTestCase, ThreeAutomatonStressTest) {
    CompleteDeterministicAutomaton automaton("aaab+a*bbbb+(abbb)*aa(b+a)+(ba)*bbaaa+b + (aba*(baba+abaab*))");
    CompleteDeterministicAutomaton minimal_automaton = automaton.CreateMinimalDeterministicAutomaton();
    CompleteDeterministicAutomaton another(minimal_automaton.CreateRegularExpression());
    const size_t word_size = 15;
    for (size_t bit_word = 0; bit_word < (1ULL << word_size); ++bit_word) {
        string regular_expression;
        for (size_t i = 0; i < word_size; ++i) {
            if (get_bit(bit_word, i))
                regular_expression += 'a';
            else
                regular_expression += 'b';
        }
        bool b1 = automaton.Contain(regular_expression);
        bool b2 = minimal_automaton.Contain(regular_expression);
        bool b3 = another.Contain(regular_expression);
        EXPECT_TRUE(b1 == b2 && b2 == b3);
    }
}
