#include <gmock/gmock.h>

#include "LINQpp/Linq.hpp"
#include "Utils/SampleClass.hpp"

TEST(LinqSelectTest, Select_ExpectVectorBuilt)
{
    std::vector<int32_t> someVector = {1, 2, 3, 4};

    std::vector<SampleClass> output = linq::from(someVector)->select([](const auto &element) -> SampleClass { return SampleClass(element); });

    EXPECT_THAT(output, testing::ElementsAre(1, 2, 3, 4));
}

TEST(LinqSelectTest, Select_AnonymousClass_ExpectVectorBuilt)
{
    std::vector<int32_t> someVector = {1, 2, 3, 4};

    std::vector output = linq::from(someVector)->select([](const auto &element) {
                                                   struct
                                                   {
                                                       int32_t a, b;
                                                   } out;

                                                   out.a = element + 1;
                                                   out.b = element + 2;

                                                   return out;
                                               })
                             ->get();

    ASSERT_EQ(someVector.size(), output.size());
    for (size_t i = 0; i < someVector.size(); i++)
    {
        auto outputElement = output.at(i);
        auto inputElement = someVector.at(i);

        EXPECT_EQ(outputElement.a, inputElement + 1);
        EXPECT_EQ(outputElement.b, inputElement + 2);
    }
}

TEST(LinqSelectTest, Select_Where_ExpectVectorBuilt)
{
    std::vector<int32_t> someVector = {1, 2, 3, 4};

    std::vector output = linq::from(someVector)
                             ->where([](const auto &element) { return element % 2 == 0; })
                             ->select([](const auto &element) {
                                 struct
                                 {
                                     int32_t a, b;
                                 } out;

                                 out.a = element + 1;
                                 out.b = element + 2;

                                 return out;
                             })
                             ->get();

    EXPECT_EQ(output.size(), 2);
    for (size_t i = 0; i < output.size(); i++)
    {
        auto outputElement = output.at(i);
        auto index = (1 + i) * 2;
        EXPECT_EQ(index, outputElement.a - 1);
        EXPECT_EQ(outputElement.a, outputElement.b - 1);
    }
}

TEST(LinqSelectTest, Select_Where_First_ExpectFirstMatching)
{
    std::vector<int32_t> someVector = {1, 2, 3, 4};

    auto output = linq::from(someVector)
                      ->where([](const auto &element) { return element % 2 == 0; })
                      ->select([](const auto &element) {
                          struct
                          {
                              int32_t a, b;
                          } out;

                          out.a = element + 1;
                          out.b = element + 2;

                          return out;
                      })
                      ->first([](const auto &element) {
                          return element.a == 3;
                      });

    EXPECT_EQ(output.a, 3);
    EXPECT_EQ(output.b, 4);
}

TEST(LinqSelectTest, Select_Where_ExpectProperSizeForBigStructure)
{
    std::vector<size_t> someVector;
    std::generate_n(std::back_inserter(someVector), 1000, [&] {
        return someVector.size();
    });

    std::vector output = linq::from(someVector)
                             ->where([](const auto &element) { return element % 2 == 0; })
                             ->select([](const auto &element) {
                                 return SampleClass(static_cast<int32_t>(element) + 2);
                             })
                             ->get();

    EXPECT_EQ(someVector.size(), 1000);
    EXPECT_EQ(output.size(), 500);
}

TEST(LinqSelectTest, Select_Where_ExpectListContainerWorking)
{
    std::list<size_t> someList;
    std::generate_n(std::back_inserter(someList), 1000, [&] {
        return someList.size();
    });

    auto output = linq::from(someList)
                             ->where([](const auto &element) { return element % 2 == 0; })
                             ->select([](const auto &element) {
                                 return SampleClass(static_cast<int32_t>(element) + 2);
                             })
                             ->get();

    EXPECT_EQ(typeid(std::list<SampleClass>), typeid(output));
}