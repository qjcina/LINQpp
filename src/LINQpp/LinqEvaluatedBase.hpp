#pragma once

namespace linq
{
    template <typename ContainerType>
    class LinqEvaluatedBase : public LinqBase<ContainerType>
    {
    public:
        explicit LinqEvaluatedBase(const ContainerType &container)
            : LinqBase<ContainerType>(mBaseContainerCopy), mBaseContainerCopy(container)
        {
        }

    private:
        const ContainerType mBaseContainerCopy;
    };
} // namespace linq