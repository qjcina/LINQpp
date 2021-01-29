#pragma once

#include <type_traits>

namespace linq
{
    namespace traits
    {
        template <typename T>
        class LinqObject;

        template <typename T, typename D = void>
        struct HasValueType : std::false_type
        {
        };

        template <typename T>
        struct HasValueType<T, typename std::void_t<typename T::value_type>> : std::true_type
        {
        };

        template <typename T>
        struct IsHandledContainer : std::bool_constant<HasValueType<T>::value>
        {
        };

        template <typename T>
        struct IsLinqObject : std::false_type
        {
        };

        template <typename T>
        struct IsLinqObject<LinqObject<T>> : std::true_type
        {
        };

        // TODO: Add checks for begin & end methods

    } // namespace traits
} // namespace linq