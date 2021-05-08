#pragma once

#include "LinqBase.hpp"
#include "LinqEvaluatedBase.hpp"

namespace linq
{
    template <typename ContainerType, template <typename ValueType, typename... Args> typename PreferredReturnType>
    class LinqEntity : public LinqBase<ContainerType, PreferredReturnType>
    {
        using BaseType = LinqBase<ContainerType, PreferredReturnType>;
        using OutputContainerType = typename BaseType::OutputContainerType;

    public:
        LinqEntity(const LinqObject<const BaseType> &parent, OutputContainerType &&outputContainer)
            : LinqBase<ContainerType, PreferredReturnType>(parent, std::move(outputContainer))
        {
        }

        LinqObjectBase<ContainerType, PreferredReturnType> forceEvaluate() const override;

        operator ContainerType() const override;
    };

    template <typename ContainerType,
              template <typename ValueType, typename... Args> typename PreferredReturnType>
    LinqObjectBase<ContainerType, PreferredReturnType> LinqEntity<ContainerType, PreferredReturnType>::forceEvaluate() const
    {
        return std::make_shared<LinqEvaluatedBase<ContainerType, PreferredReturnType>>(operator ContainerType());
    }

    template <typename ContainerType,
              template <typename ValueType, typename... Args> typename PreferredReturnType>
    LinqEntity<ContainerType, PreferredReturnType>::operator ContainerType() const
    {
        ContainerType output;
        
        // If container has reserve function (STL for example) try reserving proper amount of data
        if constexpr (linq::traits::UtilityChecks::HasReserve<decltype(output), void(size_t)>::value)
            output.reserve(LinqBase<ContainerType, PreferredReturnType>::mOutputContainer.size());

        for (const auto &elementIterator : LinqBase<ContainerType, PreferredReturnType>::mOutputContainer)
        {
            output.push_back(*elementIterator);
        }

        return output;
    }

} // namespace linq