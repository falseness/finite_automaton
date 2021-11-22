#include "practice1.h"
#include <source/practice1/parser.h>
#include <source/automatons/complete_deterministic.h>
#include <source/regular_expression/parse_reverse_polish_notation.h>
#include "tests.h"

void Practice1TestCase::SetUp() {}

void Practice1TestCase::TearDown() {}

TEST_F(Practice1TestCase, SimpleTest) {
    EXPECT_TRUE(ReversePolishNotationParser::Parse("ab+cx.*.", 2, 0) == 0);
    EXPECT_TRUE(ReversePolishNotationParser::Parse("ab+cx.*.", 2, 1) == 1);
}

TEST_F(Practice1TestCase, ModuleOneTest) {
    EXPECT_EQ(ReversePolishNotationParser::Parse("ab+z*.", 1, 0), 1);
}

TEST_F(Practice1TestCase, EmptySetLanguageTest) {
    EXPECT_EQ(ReversePolishNotationParser::Parse("00+ abc..0.+", 1, 0), 0);
}

TEST_F(Practice1TestCase, EmptyWordTest) {
    EXPECT_EQ(ReversePolishNotationParser::Parse("a1.b0.+1+", 1e9, 0), 1);
}

TEST_F(Practice1TestCase, OneSimpleRegularExpressionFullStress) {
    string expression = "zzq..aqq..+*abcdef.....+*";
    size_t k = 6000;
    for (size_t i = 0; i <= 1000; ++i) {
        EXPECT_EQ(ReversePolishNotationParser::Parse(expression, k, i), i % 3 == 0);
    }
}

TEST_F(Practice1TestCase, BigModuleTest) {
    EXPECT_TRUE(ReversePolishNotationParser::Parse("ab+c*.", 1e5, 1e5 / 2 + 7) == 1);
}

TEST_F(Practice1TestCase, IncorrectRegularExpressionTest) {
    EXPECT_TRUE(ReversePolishNotationParser::Parse("ab.c", 1e3, 5) == -1);
    EXPECT_TRUE(ReversePolishNotationParser::Parse("ab.c*++", 1e3, 5) == -1);
}

TEST_F(Practice1TestCase, CorrectRegularExpressionStress) {
    srand(0);
    for (size_t i = 0; i < 150; ++i) {
        size_t k = GetRandInt(1, 100);
        size_t l = GetRandInt(0, k - 1);
        EXPECT_FALSE(ReversePolishNotationParser::Parse(GenerateReversePolishNotationExpression(i + 1), k, l) == -1);
    }
}

TEST_F(Practice1TestCase, OneSymbolStress) {
    srand(0);
    static const size_t expression_length = 15;
    for (size_t i = 0; i < 100; ++i) {
        string reverse_polish_notation_expression = GenerateReversePolishNotationExpression(expression_length);
        string normal_notation_expression =
                ParseRegularExpressionInReversePolishNotation(reverse_polish_notation_expression);
        CompleteDeterministicAutomaton automaton =
                CompleteDeterministicAutomaton(normal_notation_expression).CreateMinimalDeterministicAutomaton();
        for (char c = 'a'; c <= 'z'; ++c) {
            if (!automaton.Contain(string(1, c)))
                continue;
            size_t k = GetRandInt(2, 1000);
            EXPECT_EQ(ReversePolishNotationParser::Parse(reverse_polish_notation_expression, k, 1), 1);
            break;
        }
    }
}

TEST_F(Practice1TestCase, SimpleExpressionStress) {
    srand(0);
    string one_word(1, GetRandInt('a', 'z'));
    string res_expression = one_word;
    for (size_t i = 3; i < 52; i += 2) {
        one_word += string(1, GetRandInt('a', 'z')) + "." + string(1, GetRandInt('a', 'z')) + ".";
        res_expression += one_word;
        res_expression += "+";
        size_t k = 2 * GetRandInt(i, 200);
        for (int j = 0; j <= i; ++j) {
            EXPECT_EQ(ReversePolishNotationParser::Parse(res_expression, k, j), j % 2);
        }
    }
}
