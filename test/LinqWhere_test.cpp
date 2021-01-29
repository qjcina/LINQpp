#include <gmock/gmock.h>

#include "LINQpp/Linq.hpp"

#include "Utils/SampleClass.hpp"

TEST(LinqWhereTest, WhereLambda_Int_ExpectFilteredOutResult)
{
    const std::vector<int32_t> sampleVector = {1,
                                               2,
                                               3,
                                               4,
                                               5,
                                               6,
                                               7,
                                               8};

    const std::vector<int32_t> outputVector = linq::from(sampleVector)->where([](const auto &element) { return element % 2 == 0; });

    ASSERT_EQ(outputVector.size(), 4);
    EXPECT_THAT(outputVector, testing::ElementsAre(2, 4, 6, 8));
}

TEST(LinqWhereTest, WhereLambda_Object_ExpectFilteredOutResult)
{
    const std::vector<SampleClass> sampleVector = {1,
                                                   2,
                                                   3,
                                                   4,
                                                   5,
                                                   6,
                                                   7,
                                                   8};

    const std::vector<SampleClass> outputVector = linq::from(sampleVector)->where([](const SampleClass &element) { return element.mSomeValue % 2 == 0; });

    ASSERT_EQ(outputVector.size(), 4);
    EXPECT_THAT(outputVector, testing::ElementsAre(2, 4, 6, 8));
}

TEST(LinqWhereTest, WhereValue_Object_ExpectFilteredOutResult)
{
    const std::vector<SampleClass> sampleVector = {1,
                                                   2,
                                                   3,
                                                   4,
                                                   5,
                                                   6,
                                                   7,
                                                   8};

    const std::vector<SampleClass> outputVector = linq::from(sampleVector)->where(3);

    ASSERT_EQ(outputVector.size(), 1);
    EXPECT_THAT(outputVector, testing::ElementsAre(3));
}

TEST(LinqWhereTest, WhereValue_Int_ExpectFilteredOutResult)
{
    const std::vector<int32_t> sampleVector = {1,
                                               2,
                                               3,
                                               4,
                                               5,
                                               6,
                                               7,
                                               8};

    const std::vector<int32_t> outputVector = linq::from(sampleVector)->where(3);

    ASSERT_EQ(outputVector.size(), 1);
    EXPECT_THAT(outputVector, testing::ElementsAre(3));
}
