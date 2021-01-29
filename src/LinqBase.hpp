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

        explicit LinqBase(const ContainerType &container)
            : mBaseContainer(container)
        {
        }

        explicit LinqBase(const LinqObject<const LinqBase<ContainerType>> &parent)
            : mBaseContainer(parent->mBaseContainer), mParent(parent)
        {
        }

        LinqObjectBase<ContainerType> where(std::function<bool(typename ContainerType::value_type)> selectFunction) const;
        LinqObjectBase<ContainerType> where(typename ContainerType::value_type selectElement) const;

        virtual LinqObjectBase<ContainerType> forceEvaluate() const;

        virtual operator ContainerType() const;

        virtual ~LinqBase() = default;

    protected:
        const std::shared_ptr<const LinqBase<ContainerType>> mParent;
        const ContainerType &mBaseContainer;
    };

    template <typename ContainerType>
    LinqObjectBase<ContainerType> LinqBase<ContainerType>::where(std::function<bool(typename ContainerType::value_type)> selectFunction) const
    {
        OutputContainerType outputContainer;

        for (ContainerType::const_iterator i = mBaseContainer.begin(); mBaseContainer.end() != i; ++i)
        {
            if (selectFunction(*i))
            {
                outputContainer.push_back(i);
            }
        }
        auto x = std::make_shared<LinqEntity<ContainerType>>(shared_from_this(), outputContainer);

        return x;
    }

    template <typename ContainerType>
    LinqObjectBase<ContainerType> LinqBase<ContainerType>::where(typename ContainerType::value_type selectElement) const
    {
        return where([&](const ContainerType::value_type &value) { return value == selectElement; });
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