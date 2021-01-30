#include <gmock/gmock.h>

#include "LINQpp/Linq.hpp"

#include "Utils/SampleClass.hpp"

TEST(LinqFirstLastTest, First_Int_ExpectReturnsFirstElement)
{
    const std::vector<int32_t> sampleVector = {1,
                                               2,
                                               3,
                                               4,
                                               5,
                                               6,
                                               7,
                                               8};

    const int32_t output = linq::from(sampleVector)->first([](const auto &element) { return element % 2 == 0; });

    EXPECT_EQ(output, 2);
}

TEST(LinqFirstLastTest, First_Object_ExpectReturnsFirstElement)
{
    const std::vector<SampleClass> sampleVector = {1,
                                                   2,
                                                   3,
                                                   4,
                                                   5,
                                                   6,
                                                   7,
                                                   8};

    const SampleClass output = linq::from(sampleVector)->first([](const auto &element) { return element % 2 == 0; });

    EXPECT_EQ(output, 2);
}

TEST(LinqFirstLastTest, FirstOrDefault_Int_ExpectReturnsFirstElement)
{
    const std::vector<int32_t> sampleVector = {1,
                                               2,
                                               3,
                                               4,
                                               5,
                                               6,
                                               7,
                                               8};

    const int32_t output = linq::from(sampleVector)->firstOrDefault([](const auto &element) { return element % 2 == 0; });

    EXPECT_EQ(output, 2);
}

TEST(LinqFirstLastTest, FirstOrDefault_Int_ExpectReturnsDefaultIfNotFound)
{
    const std::vector<int32_t> sampleVector = {1,
                                               2,
                                               3,
                                               4,
                                               5,
                                               6,
                                               7,
                                               8};

    const int32_t output = linq::from(sampleVector)->firstOrDefault([](const auto &element) { return element == 50; });

    EXPECT_EQ(output, int32_t());
}

TEST(LinqFirstLastTest, Last_Int_ExpectReturnsLastElement)
{
    const std::vector<int32_t> sampleVector = {1,
                                               2,
                                               3,
                                               4,
                                               5,
                                               6,
                                               7,
                                               8};

    const int32_t output = linq::from(sampleVector)->last([](const auto &element) { return element % 2 == 0; });

    EXPECT_EQ(output, 8);
}

TEST(LinqFirstLastTest, Last_Object_ExpectReturnsLastElement)
{
    const std::vector<SampleClass> sampleVector = {1,
                                                   2,
                                                   3,
                                                   4,
                                                   5,
                                                   6,
                                                   7,
                                                   8};

    const SampleClass output = linq::from(sampleVector)->last([](const auto &element) { return element % 2 == 0; });

    EXPECT_EQ(output, 8);
}

TEST(LinqFirstLastTest, LastOrDefault_Int_ExpectReturnsLastElement)
{
    const std::vector<int32_t> sampleVector = {1,
                                               2,
                                               3,
                                               4,
                                               5,
                                               6,
                                               7,
                                               8};

    const int32_t output = linq::from(sampleVector)->lastOrDefault([](const auto &element) { return element % 2 == 0; });

    EXPECT_EQ(output, 8);
}

TEST(LinqFirstLastTest, LastOrDefault_Int_ExpectReturnsDefaultIfNotFound)
{
    const std::vector<int32_t> sampleVector = {1,
                                               2,
                                               3,
                                               4,
                                               5,
                                               6,
                                               7,
                                               8};

    const int32_t output = linq::from(sampleVector)->lastOrDefault([](const auto &element) { return element == 50; });

    EXPECT_EQ(output, int32_t());
}

TEST(LinqFirstLastTest, First_ExpectThrowIfNotFound)
{
    const std::vector<int32_t> sampleVector = {1,
                                               2,
                                               3,
                                               4,
                                               5,
                                               6,
                                               7,
                                               8};

    EXPECT_THROW(linq::from(sampleVector)->first([](const auto &element) { return element == 50; }), std::logic_error);
}

TEST(LinqFirstLastTest, Last_ExpectThrowIfNotFound)
{
    const std::vector<int32_t> sampleVector = {1,
                                               2,
                                               3,
                                               4,
                                               5,
                                               6,
                                               7,
                                               8};

    EXPECT_THROW(linq::from(sampleVector)->last([](const auto &element) { return element == 50; }), std::logic_error);
}