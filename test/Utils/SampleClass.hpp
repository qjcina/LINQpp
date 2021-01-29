#pragma once

#include <cstdint>

struct SampleClass
{
    int32_t mSomeValue;

    SampleClass(const int32_t value)
        : mSomeValue(value)
    {
    }

    operator int32_t() const 
    {
        return mSomeValue;
    }
};