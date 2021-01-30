#pragma once

#include "LinqTypes.hpp"
#include "LinqEntity.hpp"

#include <functional>
#include <algorithm>

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

        LinqObjectBase<ContainerType> where(const Comparator &comparator) const;

        const ElementType &first(const Comparator &comparator) const;
        const ElementType &last(const Comparator &comparator) const;
        template <typename ReturnType = ElementType, std::enable_if_t<std::is_default_constructible_v<ReturnType>, bool> = true>
        ReturnType firstOrDefault(const Comparator &comparator) const;
        template <typename ReturnType = ElementType, std::enable_if_t<std::is_default_constructible_v<ReturnType>, bool> = true>
        ReturnType lastOrDefault(const Comparator &comparator) const;

        virtual LinqObjectBase<ContainerType> forceEvaluate() const;

        virtual operator ContainerType() const;

        virtual ~LinqBase() = default;

    protected:
        const std::shared_ptr<const LinqBase<ContainerType>> mParent;
        const ContainerType &mBaseContainer;
    };

    template <typename ContainerType>
    LinqObjectBase<ContainerType> LinqBase<ContainerType>::where(const Comparator &comparator) const
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
    const typename LinqBase<ContainerType>::ElementType &LinqBase<ContainerType>::first(const Comparator &comparator) const
    {
        const auto foundElement = std::find_if(mBaseContainer.begin(), mBaseContainer.end(), comparator);
        if (foundElement != mBaseContainer.end())
            return *foundElement;

        throw std::logic_error("LINQpp could not find element.");
    }

    template <typename ContainerType>
    template <typename ReturnType, std::enable_if_t<std::is_default_constructible_v<ReturnType>, bool>>
    ReturnType LinqBase<ContainerType>::firstOrDefault(const Comparator &comparator) const
    {
        const auto foundElement = std::find_if(mBaseContainer.begin(), mBaseContainer.end(), comparator);
        if (foundElement != mBaseContainer.end())
            return *foundElement;
        else
            return ElementType();
    }

    template <typename ContainerType>
    const typename LinqBase<ContainerType>::ElementType &LinqBase<ContainerType>::last(const Comparator &comparator) const
    {
        const auto foundElement = std::find_if(mBaseContainer.rbegin(), mBaseContainer.rend(), comparator);
        if (foundElement != mBaseContainer.rend())
            return *foundElement;

        throw std::logic_error("LINQpp could not find element.");
    }

    template <typename ContainerType>
    template <typename ReturnType, std::enable_if_t<std::is_default_constructible_v<ReturnType>, bool>>
    ReturnType LinqBase<ContainerType>::lastOrDefault(const Comparator &comparator) const
    {
        const auto foundElement = std::find_if(mBaseContainer.rbegin(), mBaseContainer.rend(), comparator);
        if (foundElement != mBaseContainer.rend())
            return *foundElement;
        else
            return ElementType();
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