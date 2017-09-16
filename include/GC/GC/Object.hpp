#pragma once

#include <type_traits>


namespace GC
{
    struct Object {
        virtual ~Object() = default;
    };

    template<typename T, typename Enable = void>
    struct TypedObject : public Object {};

    template<typename T>
    struct TypedObject<T, std::enable_if_t<!std::is_array<T>::value>> : public Object
    {
        using type = T;

        template<typename ... Param>
        TypedObject(Param ... params) : object(T(std::forward<Param>(params) ...)) {}

        T object;
    };

    template<typename T>
    struct TypedObject<T, std::enable_if_t<std::is_array<T>::value>> : public Object
    {
        using type = construct_array_t<T>;

        template<typename ... Param>
        TypedObject(Param ... params)
        {
            object = { params ... };
        }

        construct_array_t<T> object;
    };
}
