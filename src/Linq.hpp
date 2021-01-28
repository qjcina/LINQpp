#pragma once

#include <functional>
#include <type_traits>

#include "LinqBase.hpp"
#include "LinqEvaluatedBase.hpp"

namespace linq
{
    template <typename ContainerType>
    auto from(const ContainerType &container) -> LinqBase<ContainerType>
    {
        return LinqBase(container);
    }

    template <typename ContainerType>
    auto copyFrom(const ContainerType &container) -> LinqBase<ContainerType>
    {
        return LinqEvaluatedBase(container);
    }

}; // namespace linq