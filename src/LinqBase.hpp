#pragma once

#include "LinqTypes.hpp"
#include "LinqEntity.hpp"

#include <functional>

namespace linq
{
    template <typename ContainerType>
    class LinqBase
    {
    protected:
        using ContainerValueType = typename ContainerType::value_type;
        using OutputContainerInternalType = typename ContainerType::const_iterator;
        using OutputContainerType = std::vector<OutputContainerInternalType>;

    public:
        explicit LinqBase(const ContainerType &container)
            : mBaseContainer(container)
        {
        }

        LinqEntity<ContainerType> select(std::function<bool(typename ContainerType::value_type)> selectFunction) const;
        LinqEntity<ContainerType> select(typename ContainerType::value_type selectElement) const;

        virtual LinqBase<ContainerType> forceEvaluate() const;

        virtual operator ContainerType() const;

        virtual ~LinqBase() = default;

    protected:
        const ContainerType &mBaseContainer;
    };
    
    template <typename ContainerType>
    LinqEntity<ContainerType> LinqBase<ContainerType>::select(std::function<bool(typename ContainerType::value_type)> selectFunction) const
    {
        OutputContainerType outputContainer;

        for (ContainerType::const_iterator i = mBaseContainer.begin(); mBaseContainer.end() != i; ++i)
        {
            if (selectFunction(*i))
            {
                outputContainer.push_back(i);
            }
        }

        return LinqEntity<ContainerType>(mBaseContainer, outputContainer);
    }

    template <typename ContainerType>
    LinqEntity<ContainerType> LinqBase<ContainerType>::select(typename ContainerType::value_type selectElement) const
    {
        OutputContainerType outputContainer;

        for (ContainerType::const_iterator i = mBaseContainer.begin(); mBaseContainer.end() != i; ++i)
        {
            if (selectElement == *i)
            {
                outputContainer.push_back(i);
            }
        }

        return LinqEntity<ContainerType>(mBaseContainer, outputContainer);
    }

    template <typename ContainerType>
    LinqBase<ContainerType> LinqBase<ContainerType>::forceEvaluate() const
    {
        return LinqBase<ContainerType>(*this);
    }

    template <typename ContainerType>
    LinqBase<ContainerType>::operator ContainerType() const
    {
        return mBaseContainer;
    }

} // namespace linq