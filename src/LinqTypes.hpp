#pragma once

#include <memory>

#include "LinqUtils.hpp"

namespace linq
{
    namespace internal
    {
        template <typename ContainerType>
        class LinqEvaluatedBase;
    } // namespace internal

    template <typename ContainerType>
    class LinqEntity;

    template <typename ContainerType>
    class LinqBase;

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
                abort();
            }
        }

    private:
        template <typename I>
        std::shared_ptr<BaseType> castBaseType(const std::shared_ptr<I> &input)
        {
            if constexpr (std::is_same_v<I, BaseType>())
                return input;

            return std::static_pointer_cast<BaseType>(input);
        }
    };

    template <typename ContainerType>
    using LinqObjectBase = LinqObject<LinqBase<ContainerType>>;

} // namespace linq