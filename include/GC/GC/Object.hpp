#pragma once

#include <type_traits>


namespace GC
{
    struct Object {
        virtual ~Object() = default;
    };

    template<typename T>
    struct TypedObject : public Object
    {
        using type = T;

        template<typename ... Param>
        TypedObject(Param ... params) : object(T(std::forward<Param>(params) ...)) {}

        T object;
    };
}
