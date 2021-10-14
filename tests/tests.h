#pragma once

#include <gtest/gtest.h>

class SomeTestCase : public ::testing::Test {
    void SetUp() override;
    void TearDown() override;
};
