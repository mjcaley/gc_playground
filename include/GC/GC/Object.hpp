#pragma once

#include <type_traits>

// #include "GC/RefFwd.hpp"


namespace GC
{
    class Ptr;

    template<typename T, typename Enable> struct Ref;

    struct Object {
        virtual ~Object() = default;
    };

    template<typename T, typename Enable = void>
    class TypedObject : public Object {};

    template<typename T>
    class TypedObject<T, std::enable_if_t<!std::is_array<T>::value>> : public Object
    {
    public:
        using type = T;

        template<typename ... Param>
        TypedObject(Param ... params) : object(T(params ...)) {}

    private:
        T object;

        friend Ptr;
        friend Ref<T, typename std::enable_if_t<!std::is_array<T>::value>>;
    };

    template<typename T>
    class TypedObject<T, std::enable_if_t<std::is_array<T>::value>> : public Object
    {
    public:
        using type = construct_array_t<T>;

        template<typename ... Param>
        TypedObject(Param ... params) : object({ params ... }) {}

    private:
        construct_array_t<T> object;

        friend Ptr;
        friend Ref<T, std::enable_if_t<std::is_array<T>::value>>;
    };
}
