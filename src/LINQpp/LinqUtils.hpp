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

        namespace UtilityChecks
        {
            template <typename, typename T>
            struct HasReserve
            {
                static_assert(
                    std::integral_constant<T, false>::value,
                    "Second template parameter needs to be of function type.");
            };

            template <typename C, typename Ret, typename... Args>
            struct HasReserve<C, Ret(Args...)>
            {
            private:
                template <typename T>
                static constexpr auto check(T *)
                    -> typename std::is_same<
                        decltype(std::declval<T>().reserve(std::declval<Args>()...)),
                        Ret>::type;

                template <typename>
                static constexpr std::false_type check(...);

                typedef decltype(check<C>(0)) type;

            public:
                static constexpr bool value = type::value;
            };
        }

    } // namespace traits
} // namespace linq