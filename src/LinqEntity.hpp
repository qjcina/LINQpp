#pragma once

#include "LinqBase.hpp"
#include "LinqEvaluatedBase.hpp"

namespace linq
{
    template <typename ContainerType>
    class LinqEntity : public LinqBase<ContainerType>
    {
        using OutputContainerType = typename LinqBase<ContainerType>::OutputContainerType;

    public:
        LinqEntity(const ContainerType &baseContainer, const OutputContainerType &outputContainer)
            : LinqBase(baseContainer), mOutputContainer(outputContainer)
        {
        }

        LinqBase<ContainerType> forceEvaluate() const override;

        operator ContainerType() const override;

    private:
        const OutputContainerType mOutputContainer;
    };

    template <typename ContainerType>
    LinqBase<ContainerType> LinqEntity<ContainerType>::forceEvaluate() const
    {
        return LinqEvaluatedBase(operator ContainerType());
    }

    template <typename ContainerType>
    LinqEntity<ContainerType>::operator ContainerType() const
    {
        ContainerType output;
        output.reserve(mOutputContainer.size());

        for (const auto &elementIterator : mOutputContainer)
        {
            output.push_back(*elementIterator);
        }

        return output;
    }

} // namespace linq