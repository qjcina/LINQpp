#pragma once

#include <functional>
#include <type_traits>

#include "LinqBase.hpp"
#include "LinqEvaluatedBase.hpp"

namespace linq
{
    template <typename ContainerType>
    auto from(const ContainerType &container) -> linq::LinqObjectBase<ContainerType>
    {
        return std::make_shared<LinqBase<ContainerType>>(container);
    }

    template <typename ContainerType>
    auto copyFrom(const ContainerType &container) -> linq::LinqObjectBase<ContainerType>
    {
        return std::make_shared<LinqEvaluatedBase<ContainerType>>(container);
    }

}; // namespace linq