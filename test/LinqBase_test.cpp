#include <gmock/gmock.h>

#include "Linq.hpp"
#include "Utils/SampleClass.hpp"

TEST(LinqBaseTest, From_ExpectSuccessIfBaseIsNotModified)
{
    std::vector<int32_t> someVector = {1, 2, 3, 4};

    // skip evaluation
    auto output = linq::from(someVector)->where(3);

    const std::vector<int32_t> outputVector = output;

    EXPECT_THAT(outputVector, testing::ElementsAre(3));
}

TEST(LinqBaseTest, CopyFrom_ExpectSuccessIfBaseIsModified)
{
    std::vector<int32_t> someVector = {1, 2, 3, 4};

    // skip evaluation
    auto output = linq::copyFrom(someVector)->where(3);

    someVector.erase(std::find(someVector.begin(), someVector.end(), 4));

    const std::vector<int32_t> outputVector = output;

    EXPECT_THAT(outputVector, testing::ElementsAre(3));
}

TEST(LinqBaseTest, From_ExpectSuccessIfBaseIsModifiedAfterEvaluation)
{
    std::vector<int32_t> someVector = {1, 2, 3, 4};

    auto output = linq::from(someVector)->where(3)->forceEvaluate();

    someVector.erase(std::find(someVector.begin(), someVector.end(), 4));

    const std::vector<int32_t> outputVector = output;

    EXPECT_THAT(outputVector, testing::ElementsAre(3));
}

TEST(LinqBaseTest, From_ExpectClassVectorHandled)
{
    std::vector<SampleClass> someVector = {1, 2, 3, 4};

    auto output = linq::copyFrom(someVector)->where(3)->forceEvaluate();

    someVector.erase(std::find(someVector.begin(), someVector.end(), 4));

    const std::vector<SampleClass> outputVector = output;

    EXPECT_THAT(outputVector, testing::ElementsAre(3));
}