#pragma once


namespace linq
{
    template <typename ContainerType, template <typename ValueType, typename... Args> typename PreferredReturnType>
    class LinqEvaluatedBase : public LinqBase<ContainerType, PreferredReturnType>
    {
    public:
        explicit LinqEvaluatedBase(const ContainerType &container)
            : LinqBase<ContainerType, PreferredReturnType>(mBaseContainerCopy, true), mBaseContainerCopy(container)
        {
            LinqBase<ContainerType, PreferredReturnType>::buildOutputContainer();
        }

        explicit LinqEvaluatedBase(ContainerType &&container)
            : LinqBase<ContainerType, PreferredReturnType>(mBaseContainerCopy, true), mBaseContainerCopy(std::move(container))
        {
            LinqBase<ContainerType, PreferredReturnType>::buildOutputContainer();
        }

    private:
        const ContainerType mBaseContainerCopy;
    };
} // namespace linq