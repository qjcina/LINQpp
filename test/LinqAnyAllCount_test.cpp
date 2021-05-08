#include <gmock/gmock.h>

#include "LINQpp/Linq.hpp"
#include "Utils/SampleClass.hpp"

TEST(LinqAnyAllTest, Any_Int_ExpectTrueIfAnyElementMatches)
{
    const std::vector<int32_t> sampleVector = {1,
                                               2,
                                               3,
                                               4,
                                               5,
                                               6,
                                               7,
                                               8};

    const bool output = linq::from(sampleVector)->any([](const auto &element) { return element % 2 == 0; });

    EXPECT_TRUE(output);
}

TEST(LinqAnyAllTest, Any_Object_ExpectTrueIfAnyElementMatches)
{
    const std::vector<SampleClass> sampleVector = {1,
                                                   2,
                                                   3,
                                                   4,
                                                   5,
                                                   6,
                                                   7,
                                                   8};

    const bool output = linq::from(sampleVector)->any([](const auto &element) { return element % 2 == 0; });

    EXPECT_TRUE(output);
}

TEST(LinqAnyAllTest, Any_Int_ExpectFalseIfNoneOfElementsMatch)
{
    const std::vector<int32_t> sampleVector = {1,
                                               2,
                                               3,
                                               4,
                                               5,
                                               6,
                                               7,
                                               8};

    const bool output = linq::from(sampleVector)->any([](const auto &element) { return element == 50; });

    EXPECT_FALSE(output);
}

TEST(LinqAnyAllTest, Any_Object_ExpectFalseIfNoneOfElementsMatch)
{
    const std::vector<SampleClass> sampleVector = {1,
                                                   2,
                                                   3,
                                                   4,
                                                   5,
                                                   6,
                                                   7,
                                                   8};

    const bool output = linq::from(sampleVector)->any([](const auto &element) { return element == 50; });

    EXPECT_FALSE(output);
}

TEST(LinqAnyAllTest, All_Int_ExpectTrueIfAllElementMatch)
{
    const std::vector<int32_t> sampleVector = {1,
                                               2,
                                               3,
                                               4,
                                               5,
                                               6,
                                               7,
                                               8};

    const bool output = linq::from(sampleVector)->all([](const auto &element) { return element > 0; });

    EXPECT_TRUE(output);
}

TEST(LinqAnyAllTest, All_Object_ExpectTrueIfAllElementMatch)
{
    const std::vector<SampleClass> sampleVector = {1,
                                                   2,
                                                   3,
                                                   4,
                                                   5,
                                                   6,
                                                   7,
                                                   8};

    const bool output = linq::from(sampleVector)->all([](const auto &element) { return element > 0; });

    EXPECT_TRUE(output);
}

TEST(LinqAnyAllTest, All_Int_ExpectFalseIfAnyElementDoesNotMatch)
{
    const std::vector<int32_t> sampleVector = {1,
                                               2,
                                               3,
                                               4,
                                               5,
                                               6,
                                               7,
                                               8};

    const bool output = linq::from(sampleVector)->all([](const auto &element) { return element < 5; });

    EXPECT_FALSE(output);
}

TEST(LinqAnyAllTest, All_Object_ExpectFalseIfAnyElementDoesNotMatch)
{
    const std::vector<SampleClass> sampleVector = {1,
                                                   2,
                                                   3,
                                                   4,
                                                   5,
                                                   6,
                                                   7,
                                                   8};

    const bool output = linq::from(sampleVector)->all([](const auto &element) { return element < 5; });

    EXPECT_FALSE(output);
}

TEST(LinqAnyAllTest, Count_Int_ExpectElementCount)
{
    const std::vector<int32_t> sampleVector = {1,
                                               2,
                                               3,
                                               4,
                                               5,
                                               6,
                                               7,
                                               8};

    const size_t output = linq::from(sampleVector)->count([](const auto &element) { return element < 5; });

    EXPECT_EQ(output, 4);
}

TEST(LinqAnyAllTest, Count_Object_ExpectElementCount)
{
    const std::vector<SampleClass> sampleVector = {1,
                                               2,
                                               3,
                                               4,
                                               5,
                                               6,
                                               7,
                                               8};

    const size_t output = linq::from(sampleVector)->count([](const auto &element) { return element < 5; });

    EXPECT_EQ(output, 4);
}


TEST(LinqAnyAllTest, CountUsingLiteral_Int_ExpectElementCount)
{
    const std::vector<int32_t> sampleVector = {1,
                                               2,
                                               3,
                                               4,
                                               5,
                                               6,
                                               7,
                                               8};

    const size_t output = linq::from(sampleVector)->count(3);

    EXPECT_EQ(output, 1);
}

TEST(LinqAnyAllTest, CountUsingLiteral_Object_ExpectElementCount)
{
    const std::vector<SampleClass> sampleVector = {1,
                                               2,
                                               3,
                                               4,
                                               5,
                                               6,
                                               7,
                                               8};

    const size_t output = linq::from(sampleVector)->count(3);

    EXPECT_EQ(output, 1);
}