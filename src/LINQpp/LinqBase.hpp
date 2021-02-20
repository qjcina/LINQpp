#pragma once

#include "LinqTypes.hpp"
#include "LinqEntity.hpp"

#include <functional>
#include <algorithm>
#include <list>

namespace linq
{
    /** Base class for all LINQpp objects.
     *  Contains all methods required for modifying data and container operations.
     */
    template <typename ContainerType>
    class LinqBase : public std::enable_shared_from_this<LinqBase<ContainerType>>
    {
    protected:
        using ContainerValueType = typename ContainerType::value_type;
        using OutputContainerType = std::list<typename ContainerType::const_iterator>;

    public:
        using ValueType = ContainerType;
        using ElementType = typename ContainerType::value_type;
        using Comparator = LinqComparable<ElementType>;

        /** Basic LinqBase constructor.
         *  Assigns internal base to provided reference and builds output 
         *  container with iterators to each element of provided container.
         * \param container Handled container
         */
        explicit LinqBase(const ContainerType &container)
            : mBaseContainer(container)
        {
            buildOutputContainer();
        }
        /** Chain LinqBase constructor.
         *  References parent and moves output container.
         * \param parent Reference to another LINQpp object
         * \param outputContainer Already created output container
         */
        explicit LinqBase(const LinqObject<const LinqBase<ContainerType>> &parent, OutputContainerType &&outputContainer)
            : mBaseContainer(parent->mBaseContainer), mParent(parent), mOutputContainer(std::move(outputContainer))
        {
        }

        /** Select operation.
         *  Creates new container with transformed values.
         * \param builder Builder lambda type, it should 
         *                take current element type as argument and return new element.
         */
        template <typename LambdaType,
                  typename LambdaReturnType = std::result_of_t<LambdaType(ElementType)>,
                  typename ReturnContainerType = std::list<LambdaReturnType>,
                  typename ReturnLinqType = LinqObjectBase<typename ReturnContainerType>>
        ReturnLinqType select(const LambdaType &builder) const;

        /** Where operation. 
         *  Filters input container.
         * \param comparator Lambda or value to compare all elements with.
         */
        LinqObjectBase<ContainerType> where(const Comparator &comparator) const;

        /** First operation. 
         *  Returns first element that matches comparator.
         * \param comparator Lambda or value to compare all elements with.
         * \throws Throws logic error if no element was found.
         */
        const ElementType &first(const Comparator &comparator) const;

        /** Last operation. 
         *  Returns last element that matches comparator.
         * \param comparator Lambda or value to compare all elements with.
         * \throws Throws logic error if no element was found.
         */
        const ElementType &last(const Comparator &comparator) const;

        /** FirstOrDefault operation.
         *  Returns first element that matches comparator. 
         *  This method is available only for ElementTypes that are default constructible.
         * \param comparator Lambda or value to compare all elements with.
         */
        template <typename ReturnType = ElementType, std::enable_if_t<std::is_default_constructible_v<ReturnType>, bool> = true>
        ReturnType firstOrDefault(const Comparator &comparator) const;

        /** LastOrDefault operation.
         *  Returns last element that matches comparator. 
         *  This method is available only for ElementTypes that are default constructible.
         * \param comparator Lambda or value to compare all elements with.
         */
        template <typename ReturnType = ElementType, std::enable_if_t<std::is_default_constructible_v<ReturnType>, bool> = true>
        ReturnType lastOrDefault(const Comparator &comparator) const;

        /** Any operation.
         *  Returns true if any element matches comparator.
         * \param comparator Lambda or value to compare all elements with.
         */
        bool any(const Comparator &comparator) const;

        /** All operation.
         *  Returns true if all elements matche comparator.
         * \param comparator Lambda or value to compare all elements with.
         */
        bool all(const Comparator &comparator) const;

        /** Contains operation. Alias to any. 
         *  Returns true if any element matches comparator.
         * \param comparator Lambda or value to compare all elements with.
         */
        bool contains(const Comparator &comparator) const;

        /** Evaluates all output iterators. 
         *  This method should be used when you want to modify base 
         *  container in-between LINQpp operations.
         */
        virtual LinqObjectBase<ContainerType> forceEvaluate() const;

        /** Casts LinqBase to ContainerType.
         */
        virtual operator ContainerType() const;

        /** Casts LinqBase to ContainerType.
         *  Might be used when LinqObject is returning anonymous class
         *  or to force cast.
         */
        ContainerType get() const
        {
            return this->operator ContainerType();
        }

        virtual ~LinqBase() = default;

    protected:
        /** LinqBase constructor that skips building output container.
         * \param container Handled container
         */
        LinqBase(const ContainerType &container, bool)
            : mBaseContainer(container)
        {
        }

        void buildOutputContainer()
        {
            auto iterator = mBaseContainer.begin();
            while (iterator != mBaseContainer.end())
            {
                mOutputContainer.push_back(iterator++);
            }
        }

        const std::shared_ptr<const LinqBase<ContainerType>> mParent;
        const ContainerType &mBaseContainer;
        OutputContainerType mOutputContainer;
    };

    template <typename ContainerType>
    template <typename LambdaType,
              typename LambdaReturnType,
              typename ReturnContainerType,
              typename ReturnLinqType>
    ReturnLinqType LinqBase<ContainerType>::select(const LambdaType &builder) const
    {
        ReturnContainerType outputContainer;
        for (auto i = mOutputContainer.begin(); mOutputContainer.end() != i; ++i)
        {
            outputContainer.push_back(builder(**i));
        }
        return moveFrom(std::move(outputContainer));
    }

    template <typename ContainerType>
    LinqObjectBase<ContainerType> LinqBase<ContainerType>::where(const Comparator &comparator) const
    {
        OutputContainerType outputContainer;
        for (auto i = mOutputContainer.begin(); mOutputContainer.end() != i; ++i)
        {
            if (comparator(*i))
            {
                outputContainer.push_back(*i);
            }
        }

        return std::make_shared<LinqEntity<ContainerType>>(this->shared_from_this(), std::move(outputContainer));
    }

    template <typename ContainerType>
    const typename LinqBase<ContainerType>::ElementType &LinqBase<ContainerType>::first(const Comparator &comparator) const
    {
        const auto foundElement = std::find_if(mOutputContainer.begin(), mOutputContainer.end(), comparator);
        if (foundElement != mOutputContainer.end())
            return **foundElement;

        throw std::logic_error("LINQpp could not find element.");
    }

    template <typename ContainerType>
    template <typename ReturnType, std::enable_if_t<std::is_default_constructible_v<ReturnType>, bool>>
    ReturnType LinqBase<ContainerType>::firstOrDefault(const Comparator &comparator) const
    {
        const auto foundElement = std::find_if(mOutputContainer.begin(), mOutputContainer.end(), comparator);
        if (foundElement != mOutputContainer.end())
            return **foundElement;
        else
            return ElementType();
    }

    template <typename ContainerType>
    const typename LinqBase<ContainerType>::ElementType &LinqBase<ContainerType>::last(const Comparator &comparator) const
    {
        const auto foundElement = std::find_if(mOutputContainer.rbegin(), mOutputContainer.rend(), comparator);
        if (foundElement != mOutputContainer.rend())
            return **foundElement;

        throw std::logic_error("LINQpp could not find element.");
    }

    template <typename ContainerType>
    template <typename ReturnType, std::enable_if_t<std::is_default_constructible_v<ReturnType>, bool>>
    ReturnType LinqBase<ContainerType>::lastOrDefault(const Comparator &comparator) const
    {
        const auto foundElement = std::find_if(mOutputContainer.rbegin(), mOutputContainer.rend(), comparator);
        if (foundElement != mOutputContainer.rend())
            return **foundElement;
        else
            return ElementType();
    }

    template <typename ContainerType>
    bool LinqBase<ContainerType>::any(const Comparator &comparator) const
    {
        return std::any_of(mOutputContainer.begin(), mOutputContainer.end(), comparator);
    }

    template <typename ContainerType>
    bool LinqBase<ContainerType>::all(const Comparator &comparator) const
    {
        return std::all_of(mOutputContainer.begin(), mOutputContainer.end(), comparator);
    }

    template <typename ContainerType>
    bool LinqBase<ContainerType>::contains(const Comparator &comparator) const
    {
        return any(comparator);
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