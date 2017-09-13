#pragma once

#include <memory>
#include <type_traits>

#include "GC/Collector.hpp"
#include "GC/construct_array.hpp"

namespace GC 
{
    struct Ptr;

    template<typename T, typename Enable> class TypedObject;

    struct RefBase
    {
        RefBase() = default;
        RefBase(const RefBase& ref) : ptr(ref.ptr) { ptr->reference(); }
        RefBase(Ptr* pointer) : ptr(pointer) { ptr->reference(); }
        virtual ~RefBase() { ptr->dereference(); };

    protected:
        Ptr* ptr;
    };

    template<typename T, typename Enable = void>
    struct Ref;

    template<typename T>
    struct Ref<T, std::enable_if_t<!std::is_array<T>::value>> : public RefBase
    {
        Ref(const Ref& ref) : RefBase(ref) {}
        Ref(Ptr *pointer) : RefBase(pointer) {}
        template<typename ... Param>
        Ref(Param... params)
        {
            auto pointer = Ptr::create<T>(params ...);
            pointer.reference();
            ptr = add(pointer);
        }

        T &operator*()
        {
            auto *typed_obj = static_cast<TypedObject<T>*>(ptr->get_object());
            return typed_obj->object;
        }

        T *operator->()
        {
            auto *typed_obj = static_cast<TypedObject<T>*>(ptr->get_object());
            return &typed_obj->object;
        }
    };

    template<typename T>
    struct Ref<T, std::enable_if_t<std::is_array<T>::value>> : public RefBase
    {
        Ref(const Ref& ref) : RefBase(ref) {}
        Ref(Ptr *pointer) : RefBase(pointer) {}
        template<typename ... Param>
        Ref(Param ... params)
        {
            auto pointer = Ptr::create<T>(params ...);
            pointer.reference();
            ptr = add(pointer);
        }

        construct_array_t<std::remove_extent_t<T>>& operator[](std::ptrdiff_t n)
        {
            auto* typed_obj = static_cast<TypedObject<T>*>(ptr->get_object());
            return typed_obj->object[n];
        }

        construct_array_t<std::remove_extent_t<T>>& at(std::ptrdiff_t n)
        {
            auto* typed_obj = static_cast<TypedObject<T>*>(ptr->get_object());
            return typed_obj->object.at(n);
        }

        T& operator*() {
            auto *typed_obj = static_cast<TypedObject<T> *>(ptr->get_object());
            return typed_obj->object;
        }

        T* operator->() {
            auto *typed_obj = static_cast<TypedObject<T> *>(ptr->get_object());
            return &typed_obj->object;
        }
    };
}