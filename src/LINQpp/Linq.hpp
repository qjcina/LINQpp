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
    template <typename ContainerType>
    auto from(const ContainerType &container) -> linq::LinqObjectBase<ContainerType>
    {
        return std::make_shared<LinqBase<ContainerType>>(container);
    }

    /** LINQpp entry point. Creates new LinqObject that copies provided container.
     * \param container Handled container
     */
    template <typename ContainerType>
    auto copyFrom(const ContainerType &container) -> linq::LinqObjectBase<ContainerType>
    {
        return std::make_shared<LinqEvaluatedBase<ContainerType>>(container);
    }

    /** LINQpp entry point. Creates new LinqObject that moves provided container.
     * \param container Handled container
     */
    template <typename ContainerType>
    auto moveFrom(ContainerType &&container) -> linq::LinqObjectBase<ContainerType>
    {
        return std::make_shared<LinqEvaluatedBase<ContainerType>>(std::move(container));
    }

}; // namespace linq