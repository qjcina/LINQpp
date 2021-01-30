#pragma once

#include "LinqTypes.hpp"
#include "LinqEntity.hpp"

#include <functional>

namespace linq
{
    template <typename ContainerType>
    class LinqBase : public std::enable_shared_from_this<LinqBase<ContainerType>>
    {
    protected:
        using ContainerValueType = typename ContainerType::value_type;
        using OutputContainerInternalType = typename ContainerType::const_iterator;
        using OutputContainerType = std::vector<OutputContainerInternalType>;

    public:
        using ValueType = ContainerType;
        using ElementType = typename ContainerType::value_type;
        using Comparator = LinqComparable<ElementType>;

        explicit LinqBase(const ContainerType &container)
            : mBaseContainer(container)
        {
        }

        explicit LinqBase(const LinqObject<const LinqBase<ContainerType>> &parent)
            : mBaseContainer(parent->mBaseContainer), mParent(parent)
        {
        }

        LinqObjectBase<ContainerType> where(const Comparator& comparator) const;

        virtual LinqObjectBase<ContainerType> forceEvaluate() const;

        virtual operator ContainerType() const;

        virtual ~LinqBase() = default;

    protected:
        const std::shared_ptr<const LinqBase<ContainerType>> mParent;
        const ContainerType &mBaseContainer;
    };

    template <typename ContainerType>
    LinqObjectBase<ContainerType> LinqBase<ContainerType>::where(const Comparator& comparator) const
    {
        OutputContainerType outputContainer;

        for (typename ContainerType::const_iterator i = mBaseContainer.begin(); mBaseContainer.end() != i; ++i)
        {
            if (comparator(*i))
            {
                outputContainer.push_back(i);
            }
        }
        return std::make_shared<LinqEntity<ContainerType>>(this->shared_from_this(), outputContainer);
    }

    template <typename ContainerType>
    LinqObjectBase<ContainerType> LinqBase<ContainerType>::forceEvaluate() const
    {
        // nothing to evaluate
        return std::make_shared<LinqBase<ContainerType>>(*this);
    }

    template <typename ContainerType>
    LinqBase<ContainerType>::operator ContainerType() const
    {
        return mBaseContainer;
    }

} // namespace linq