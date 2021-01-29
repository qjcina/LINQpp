#include "LINQpp/Linq.hpp"

#include <vector>
#include <iostream>

void printVector(const std::vector<int32_t> &vector)
{
    std::cout << "{ ";
    for (const int32_t &element : vector)
    {
        std::cout << element << " ";
    }
    std::cout << "}" << std::endl;
}

int main(int argc, char **argv)
{
    // Usage example

    const std::vector<int32_t> someVector = {1, 2, 3, 4};

    const std::vector<int32_t> evenNumbers = linq::from(someVector)->where([](const auto &element) { return element % 2 == 0; });

    std::cout << "All numbers: ";
    printVector(someVector);

    std::cout << "Even numbers: ";
    printVector(evenNumbers);

    return 0;
}
