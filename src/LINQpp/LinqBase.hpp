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
    template <typename ContainerType, template <typename ValueType, typename... Args> typename PreferredReturnType>
    class LinqBase : public std::enable_shared_from_this<LinqBase<ContainerType, PreferredReturnType>>
    {
        static_assert(linq::traits::IsHandledContainer<ContainerType>::value, "Provided container needs to have value_type and push_back function.");

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
        explicit LinqBase(const LinqObject<const LinqBase<ContainerType, PreferredReturnType>> &parent, OutputContainerType &&outputContainer)
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
                  typename ReturnContainerType = PreferredReturnType<LambdaReturnType>,
                  typename ReturnLinqType = LinqObjectBase<ReturnContainerType, PreferredReturnType>>
        ReturnLinqType select(const LambdaType &builder) const;

        /** Where operation. 
         *  Filters input container.
         * \param comparator Lambda or value to compare all elements with.
         */
        LinqObjectBase<ContainerType, PreferredReturnType> where(const Comparator &comparator) const;

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
        virtual LinqObjectBase<ContainerType, PreferredReturnType> forceEvaluate() const;

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

        const std::shared_ptr<const LinqBase<ContainerType, PreferredReturnType>> mParent;
        const ContainerType &mBaseContainer;
        OutputContainerType mOutputContainer;
    };

    template <typename ContainerType, template <typename ValueType, typename... Args> typename PreferredReturnType>
    template <typename LambdaType,
              typename LambdaReturnType,
              typename ReturnContainerType,
              typename ReturnLinqType>
    ReturnLinqType LinqBase<ContainerType, PreferredReturnType>::select(const LambdaType &builder) const
    {
        ReturnContainerType outputContainer;
        for (auto i = mOutputContainer.begin(); mOutputContainer.end() != i; ++i)
        {
            outputContainer.push_back(builder(**i));
        }
        return std::make_shared<LinqEvaluatedBase<ReturnContainerType, PreferredReturnType>>(std::move(outputContainer));
        ;
    }

    template <typename ContainerType, template <typename ValueType, typename... Args> typename PreferredReturnType>
    LinqObjectBase<ContainerType, PreferredReturnType> LinqBase<ContainerType, PreferredReturnType>::where(const Comparator &comparator) const
    {
        OutputContainerType outputContainer;
        for (auto i = mOutputContainer.begin(); mOutputContainer.end() != i; ++i)
        {
            if (comparator(*i))
            {
                outputContainer.push_back(*i);
            }
        }

        return std::make_shared<LinqEntity<ContainerType, PreferredReturnType>>(this->shared_from_this(), std::move(outputContainer));
    }

    template <typename ContainerType, template <typename ValueType, typename... Args> typename PreferredReturnType>
    const typename LinqBase<ContainerType, PreferredReturnType>::ElementType &LinqBase<ContainerType, PreferredReturnType>::first(const Comparator &comparator) const
    {
        const auto foundElement = std::find_if(mOutputContainer.begin(), mOutputContainer.end(), comparator);
        if (foundElement != mOutputContainer.end())
            return **foundElement;

        throw std::logic_error("LINQpp could not find element.");
    }

    template <typename ContainerType, template <typename ValueType, typename... Args> typename PreferredReturnType>
    template <typename ReturnType, std::enable_if_t<std::is_default_constructible_v<ReturnType>, bool>>
    ReturnType LinqBase<ContainerType, PreferredReturnType>::firstOrDefault(const Comparator &comparator) const
    {
        const auto foundElement = std::find_if(mOutputContainer.begin(), mOutputContainer.end(), comparator);
        if (foundElement != mOutputContainer.end())
            return **foundElement;
        else
            return ElementType();
    }

    template <typename ContainerType, template <typename ValueType, typename... Args> typename PreferredReturnType>
    const typename LinqBase<ContainerType, PreferredReturnType>::ElementType &LinqBase<ContainerType, PreferredReturnType>::last(const Comparator &comparator) const
    {
        const auto foundElement = std::find_if(mOutputContainer.rbegin(), mOutputContainer.rend(), comparator);
        if (foundElement != mOutputContainer.rend())
            return **foundElement;

        throw std::logic_error("LINQpp could not find element.");
    }

    template <typename ContainerType, template <typename ValueType, typename... Args> typename PreferredReturnType>
    template <typename ReturnType, std::enable_if_t<std::is_default_constructible_v<ReturnType>, bool>>
    ReturnType LinqBase<ContainerType, PreferredReturnType>::lastOrDefault(const Comparator &comparator) const
    {
        const auto foundElement = std::find_if(mOutputContainer.rbegin(), mOutputContainer.rend(), comparator);
        if (foundElement != mOutputContainer.rend())
            return **foundElement;
        else
            return ElementType();
    }

    template <typename ContainerType, template <typename ValueType, typename... Args> typename PreferredReturnType>
    bool LinqBase<ContainerType, PreferredReturnType>::any(const Comparator &comparator) const
    {
        return std::any_of(mOutputContainer.begin(), mOutputContainer.end(), comparator);
    }

    template <typename ContainerType, template <typename ValueType, typename... Args> typename PreferredReturnType>
    bool LinqBase<ContainerType, PreferredReturnType>::all(const Comparator &comparator) const
    {
        return std::all_of(mOutputContainer.begin(), mOutputContainer.end(), comparator);
    }

    template <typename ContainerType, template <typename ValueType, typename... Args> typename PreferredReturnType>
    bool LinqBase<ContainerType, PreferredReturnType>::contains(const Comparator &comparator) const
    {
        return any(comparator);
    }

    template <typename ContainerType, template <typename ValueType, typename... Args> typename PreferredReturnType>
    LinqObjectBase<ContainerType, PreferredReturnType> LinqBase<ContainerType, PreferredReturnType>::forceEvaluate() const
    {
        // nothing to evaluate
        return std::make_shared<LinqBase<ContainerType, PreferredReturnType>>(*this);
    }

    template <typename ContainerType, template <typename ValueType, typename... Args> typename PreferredReturnType>
    LinqBase<ContainerType, PreferredReturnType>::operator ContainerType() const
    {
        return mBaseContainer;
    }

} // namespace linq