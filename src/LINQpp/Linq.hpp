#pragma once

#include <functional>
#include <type_traits>

#include "LinqBase.hpp"
#include "LinqEvaluatedBase.hpp"

namespace linq
{
    /** LINQpp entry point. Creates new LinqObject that references proviced container.
     * \param container Handled container
     */
    template <template <typename X, typename... T> typename ContainerOuterType,
              typename ContainerValueType,
              typename... Args,
              typename ContainerType = ContainerOuterType<ContainerValueType, Args...>>
    auto from(const ContainerOuterType<ContainerValueType, Args...> &container) -> linq::LinqObjectBase<ContainerType, ContainerOuterType>
    {
        return std::make_shared<LinqBase<ContainerType, ContainerOuterType>>(container);
    }

    /** LINQpp entry point. Creates new LinqObject that copies provided container.
     * \param container Handled container
     */
    template <template <typename X, typename... T> typename ContainerOuterType,
              typename ContainerValueType,
              typename... Args,
              typename ContainerType = ContainerOuterType<ContainerValueType, Args...>>
    auto copyFrom(const ContainerOuterType<ContainerValueType, Args...> container) -> linq::LinqObjectBase<ContainerType, ContainerOuterType>
    {
        return std::make_shared<LinqEvaluatedBase<ContainerType, ContainerOuterType>>(container);
    }

    /** LINQpp entry point. Creates new LinqObject that moves provided container.
     * \param container Handled container
     */
    template <template <typename X, typename... T> typename ContainerOuterType,
              typename ContainerValueType,
              typename... Args,
              typename ContainerType = ContainerOuterType<ContainerValueType, Args...>>
    auto moveFrom(ContainerOuterType<ContainerValueType, Args...> &&container) -> linq::LinqObjectBase<ContainerType, ContainerOuterType>
    {
        return std::make_shared<LinqEvaluatedBase<ContainerType, ContainerOuterType>>(std::move(container));
    }

}; // namespace linq