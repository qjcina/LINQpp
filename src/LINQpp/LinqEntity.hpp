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
        LinqEntity(const LinqObject<const LinqBase<ContainerType>> &parent, OutputContainerType&& outputContainer)
            : LinqBase<ContainerType>(parent, std::move(outputContainer))
        {
        }

        LinqObjectBase<ContainerType> forceEvaluate() const override;

        operator ContainerType() const override;
    };

    template <typename ContainerType>
    LinqObjectBase<ContainerType> LinqEntity<ContainerType>::forceEvaluate() const
    {
        return std::make_shared<LinqEvaluatedBase<ContainerType>>(operator ContainerType());
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