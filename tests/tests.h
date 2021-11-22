#pragma once

#include <gtest/gtest.h>

class SomeTestCase : public ::testing::Test {
    void SetUp() override;
    void TearDown() override;
};

int GetRandInt(int, int);
std::string GenerateReversePolishNotationExpression(size_t iteration_count);