#pragma once

#include <memory>
#include <variant>

#include "LinqUtils.hpp"

namespace linq
{
    namespace internal
    {
        template <typename ContainerType>
        class LinqEvaluatedBase;
    } // namespace internal

    template <typename ContainerType, template <typename ValueType, typename... Args> typename PreferredReturnType>
    class LinqEntity;

    template <typename ContainerType, template <typename ValueType, typename... Args> typename PreferredReturnType>
    class LinqBase;

    template <typename ContainerType, template <typename ValueType, typename... Args> typename PreferredReturnType>
    class LinqEvaluatedBase;

    template <typename BaseType>
    class LinqObject : public std::shared_ptr<BaseType>
    {
    public:
        template <typename InputBaseType>
        LinqObject(const std::shared_ptr<InputBaseType> &base)
            : std::shared_ptr<BaseType>(castBaseType(base))
        {
        }

        template <typename InputBaseType>
        LinqObject(std::shared_ptr<InputBaseType> &&base)
            : std::shared_ptr<BaseType>(std::move(base))
        {
        }

        LinqObject() = default;

        template <typename ContainerType = typename BaseType::ValueType, std::enable_if_t<traits::IsHandledContainer<ContainerType>::value, bool> = true>
        operator ContainerType() const
        {
            if (const auto *internalPointer = this->get())
            {
                // object exists, can safely evaluate
                return internalPointer->operator ContainerType();
            }
            else if constexpr (std::is_default_constructible_v<ContainerType>)
            {
                // object doesn't exist, return enmpty container
                return ContainerType();
            }
            else
            {
                // object doesn't exist, and pointer couldn't return empty container
                throw std::logic_error("LinqObject is invalid.");
            }
        }

    private:
        template <typename I>
        std::shared_ptr<BaseType> castBaseType(const std::shared_ptr<I> &input)
        {
            if constexpr (std::is_same_v<I, BaseType>)
                return input;

            return std::static_pointer_cast<BaseType>(input);
        }
    };

    template <typename ValueType>
    class LinqComparable
    {
    public:
        using ComparatorType = std::function<bool(const ValueType &)>;

        // literal value conversion to lambda
        LinqComparable(const ValueType &literal)
            : LinqComparable<ValueType>([=](const ValueType &element) { return element == literal; })
        {
        }

        // conversion from type different than ValueType
        template <typename Type,
                  std::enable_if_t<std::is_convertible_v<Type, ValueType> && !std::is_same_v<ValueType, Type>, bool> = true>
        LinqComparable(const Type &value)
            : LinqComparable<ValueType>(static_cast<ValueType>(value))
        {
        }

        LinqComparable(const ComparatorType &function)
            : mComparator(function)
        {
        }

        // plain lambda
        template <typename LambdaType, std::enable_if_t<std::is_convertible_v<LambdaType, ComparatorType> && !std::is_same_v<LambdaType, ComparatorType>, bool> = true>
        LinqComparable(const LambdaType &lambda)
            : LinqComparable<ValueType>(static_cast<ComparatorType>(lambda))
        {
        }

        bool operator()(const ValueType &value) const
        {
            return mComparator(value);
        }

        template<typename IteratorType, std::enable_if_t<std::is_same_v<ValueType, typename IteratorType::value_type> 
                                                        && !std::is_same_v<ValueType, IteratorType>, bool> = true>
        bool operator()(const IteratorType &value) const
        {
            return mComparator(*value);
        }

    private:
        const ComparatorType mComparator;
    };

    template <typename ContainerType, template <typename ValueType> typename PreferredReturnType>
    using LinqObjectBase = LinqObject<LinqBase<ContainerType, PreferredReturnType>>;

} // namespace linq