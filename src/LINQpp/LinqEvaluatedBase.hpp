#pragma once

#include "LinqBase.hpp"

namespace linq
{
    template <typename ContainerType>
    class LinqEvaluatedBase : public LinqBase<ContainerType>
    {
    public:
        explicit LinqEvaluatedBase(const ContainerType &container)
            : LinqBase<ContainerType>(mBaseContainerCopy, true), mBaseContainerCopy(container)
        {
            LinqBase<ContainerType>::buildOutputContainer(mBaseContainerCopy);
        }

        explicit LinqEvaluatedBase(ContainerType &&container)
            : LinqBase<ContainerType>(mBaseContainerCopy, true), mBaseContainerCopy(std::move(container))
        {
            LinqBase<ContainerType>::buildOutputContainer(mBaseContainerCopy);
        }

    private:
        const ContainerType mBaseContainerCopy;
    };
} // namespace linq