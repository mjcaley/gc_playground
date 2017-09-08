#pragma once

#include <memory>

#include "GC/GC.hpp"


namespace GC {
    template<typename T>
    struct ValueObject;
    template<typename T>
    struct WeakRef;

    template<typename T>
    struct Ref {
        template<typename ... Params>
        Ref(Params... values) {
            auto object = new_object<T>(values...);
            ptr = object.get();
            ptr->reference();
            GC::GC::add_to_gc(std::move(object));
        }

        Ref(ValueObject<T> *ptr) : ptr(ptr) { ptr->reference(); }

        Ref(const Ref<T> &ref) : ptr(ref.ptr) { ptr->reference(); }

        ~Ref() { ptr->dereference(); }

        operator WeakRef<T>() { return get_weak_reference(); }

        WeakRef<T> get_weak_reference() const { return WeakRef<T>(ptr); }

        T &operator*() {
            return *ptr->ptr;
        }

        T *operator->() {
            return ptr->ptr.get();
        }

        void mark(unsigned int new_mark) { ptr->mark(new_mark); }

    private:
        ValueObject<T> *ptr;

        friend WeakRef<T>;
    };

    struct RefBase
    {
        RefBase() = default;
        RefBase(const RefBase& ref) : ptr(ref.ptr) { ptr->reference(); }
        RefBase(Ptr *pointer) : ptr(pointer) { ptr->reference(); }
        virtual ~RefBase() { ptr->dereference(); };

    protected:
        Ptr* ptr;
    };

    template<typename T>
    struct Ref2 : public RefBase
    {
        Ref2(const Ref2& ref) : RefBase(ref) {}
        Ref2(Ptr *pointer) : RefBase(pointer) {}
        template<typename ... Param>
        Ref2(Param... params) {

        }

        T &operator*() {
            auto *typed_obj = static_cast<TypedObject<T> *>(ptr->get_object());
            return typed_obj->object;
        }

        T *operator->() {
            auto *typed_obj = static_cast<TypedObject<T> *>(ptr->get_object());
            return &typed_obj->object;
        }
    };

    template<typename T, std::size_t S>
    struct Ref2<T[S]> : public RefBase
    {
        Ref2(const Ref2& ref) : RefBase(ref) {}
        Ref2(Ptr *pointer) : RefBase(pointer) {}
        template<typename ... Param>
        Ref2(Param ... params)
        {

        }

        T& operator[](std::ptrdiff_t n)
        {
            auto* typed_obj = static_cast<TypedObject<std::array<T, S>>*>(ptr->get_object());
            return typed_obj->object[n];
        }

        std::array<T, S>& operator*() {
            auto *typed_obj = static_cast<TypedObject<std::array<T, S>> *>(ptr->get_object());
            return typed_obj->object;
        }

        std::array<T, S>* operator->() {
            auto *typed_obj = static_cast<TypedObject<std::array<T, S>> *>(ptr->get_object());
            return &typed_obj->object;
        }
    };
}