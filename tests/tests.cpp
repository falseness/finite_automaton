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

TEST_F(SomeTestCase, FourAutomatonsThreeLettersCheck) {
    std::string regular_expression = "(ab(c)*+da)+hi+(ab)*(c*d)*+fef+(da)";
    // add some chars for full alphabet
    regular_expression += '+';
    for (char c = 'a'; c <= 'z'; ++c) {
        regular_expression += c;
    }
    CompleteDeterministicAutomaton automaton(regular_expression);
    CompleteDeterministicAutomaton minimal_automaton = automaton.CreateMinimalDeterministicAutomaton();
    CompleteDeterministicAutomaton another(minimal_automaton.CreateRegularExpression());
    CompleteDeterministicAutomaton complement_automaton = automaton.CreateComplement();

    for (char c1 = 'a'; c1 <= 'z'; ++c1) {
        for (char c2 = 'a'; c2 <= 'z'; ++c2) {
            for (char c3 = 'a'; c3 <= 'z'; ++c3) {
                string word;
                word += c1;
                word += c2;
                word += c3;
                bool b1 = automaton.Contain(word);
                bool b2 = minimal_automaton.Contain(word);
                bool b3 = another.Contain(word);
                bool b4 = complement_automaton.Contain(word);
                EXPECT_TRUE(b1 == b2 && b2 == b3 && b4 != b1);
            }
        }
    }
}

bool get_bit(size_t mask, size_t i) {
    return (mask >> i) & 1;
}

TEST_F(SomeTestCase, FourAutomatonStressTest) {
    CompleteDeterministicAutomaton automaton("aaab+a*bbbb+(abbb)*aa(b+a)+(ba)*bbaaa+b + (aba*(baba+abaab*))");
    CompleteDeterministicAutomaton minimal_automaton = automaton.CreateMinimalDeterministicAutomaton();
    CompleteDeterministicAutomaton another(minimal_automaton.CreateRegularExpression());
    CompleteDeterministicAutomaton complement_automaton = minimal_automaton.CreateComplement();

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
        bool b4 = complement_automaton.Contain(regular_expression);
        EXPECT_TRUE(b1 == b2 && b2 == b3 && b4 != b1);
    }
}

TEST_F(SomeTestCase, StressMinimalDFATest) {
    CompleteDeterministicAutomaton automaton("aaab*(a+bbbb*ccc)(zzzzx*+oooo)*(a+l(f+qqz))+ee*cc+asp(da*+qmmm)*");
    CompleteDeterministicAutomaton minimal_automaton = automaton.CreateMinimalDeterministicAutomaton();

    size_t min_size = minimal_automaton.size();
    const size_t iteration_count = 20;
    for (size_t i = 0; i < iteration_count; ++i) {
        minimal_automaton = std::move(CompleteDeterministicAutomaton(
                minimal_automaton.CreateRegularExpression()).CreateMinimalDeterministicAutomaton());
        EXPECT_TRUE(minimal_automaton.size() == min_size);
    }
}

TEST_F(SomeTestCase, StressCompleteDFATest) {
    string regular_expression = "z";
    const size_t iteration_count = 17;
    string string_contain = "z";
    for (char i = 0; i < iteration_count; ++i) {
        CompleteDeterministicAutomaton automaton(regular_expression);

        EXPECT_TRUE(automaton.Contain(string_contain) &&
            automaton.CreateMinimalDeterministicAutomaton().Contain(string_contain) &&
            !automaton.CreateComplement().Contain(string_contain));

        regular_expression = "(" + regular_expression + ")*";
        char new_char = static_cast<char>(i + 'a');

        regular_expression += new_char;
        string_contain += string_contain;
        string_contain += new_char;
    }
}


