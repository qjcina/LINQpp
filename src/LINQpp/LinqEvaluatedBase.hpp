#pragma once

#include "LinqTypes.hpp"

namespace linq
{
    template <typename ContainerType>
    class LinqEvaluatedBase : public LinqBase<ContainerType>
    {
    public:
        explicit LinqEvaluatedBase(const ContainerType &container)
            : LinqBase<ContainerType>(mBaseContainerCopy, true), mBaseContainerCopy(container)
        {
            LinqBase<ContainerType>::buildOutputContainer();
        }

        explicit LinqEvaluatedBase(ContainerType &&container)
            : LinqBase<ContainerType>(mBaseContainerCopy, true), mBaseContainerCopy(std::move(container))
        {
            LinqBase<ContainerType>::buildOutputContainer();
        }

    private:
        const ContainerType mBaseContainerCopy;
    };
} // namespace linq