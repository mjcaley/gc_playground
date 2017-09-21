#pragma once

#include <memory>
#include <type_traits>

#include "GC/Collector.hpp"
#include "GC/construct_array.hpp"


namespace GC 
{
    template<typename T> struct WeakRef;

    template<typename T>
    struct RefBase
    {
        RefBase() = default;
        RefBase(const RefBase& ref) : ptr(ref.ptr) { ptr->reference(); }
        RefBase(Ptr* pointer) : ptr(pointer) { ptr->reference(); }
        RefBase(const WeakRef<T>& ref) : ptr(ref.ptr) { ptr->reference(); }
        virtual ~RefBase() { ptr->dereference(); };

        Ptr* get() { return ptr; }

        WeakRef<T> to_weak_ref() { return WeakRef<T>(ptr); }

        T& operator*()
        {
            auto *typed_obj = static_cast<TypedObject<T>*>(ptr->get_object());
            return typed_obj->object;
        }

        T* operator->()
        {
            auto *typed_obj = static_cast<TypedObject<T>*>(ptr->get_object());
            return &typed_obj->object;
        }

    protected:
        Ptr* ptr;

        friend WeakRef<T>;
    };

    template<typename T, typename Enable = void>
    struct Ref : public RefBase<T> {};

    template<typename T>
    struct Ref<T, std::enable_if_t<!std::is_array<T>::value>> : public RefBase<T>
    {
        using type = T;

        Ref(const Ref& ref) : RefBase<type>(ref) {}
        Ref(Ptr* pointer) : RefBase<type>(pointer) {}
        Ref(const WeakRef<type>& ref) : RefBase<type>(ref.ptr) {}
        template<typename ... Param>
        Ref(Param... params)
        {
            auto pointer = Ptr::create<type>(std::forward<Param>(params) ...);
            pointer.reference();
            this->ptr = add(std::move(pointer));
        }

    private:
        friend WeakRef<type>;
        friend void traverse<Ref<T>>(Ref<type>&, unsigned int);
    };

    template<typename T>
    struct Ref<T, std::enable_if_t<std::is_array<T>::value>> : public RefBase<construct_array_t<T>>
    {
        using type = construct_array_t<T>;
        using child_type = construct_array_t<std::remove_extent_t<T>>;

        Ref(const Ref& ref) : RefBase<type>(ref) {}
        Ref(Ptr *pointer) : RefBase<type>(pointer) {}
        Ref(const WeakRef<type>& ref) : RefBase<type>(ref.ptr) {}
        Ref()
        {
            auto pointer = Ptr::create<type>();
            pointer.reference();
            this->ptr = add(std::move(pointer));
        }

        child_type& operator[](std::ptrdiff_t n)
        {
            auto* typed_obj = static_cast<TypedObject<type>*>(this->ptr->get_object());
            return typed_obj->object[n];
        }

        child_type& at(std::ptrdiff_t n)
        {
            auto* typed_obj = static_cast<TypedObject<type>*>(this->ptr->get_object());
            return typed_obj->object.at(n);
        }
        
    private:
        friend WeakRef<type>;
        friend void traverse<Ref<type>>(Ref<type>&, unsigned int);
    };
}
