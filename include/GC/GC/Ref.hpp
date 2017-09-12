#pragma once

#include <memory>
#include <type_traits>

//#include "GC/GC.hpp"
#include "GC/Collector.hpp"

namespace GC {
//    template<typename T>
//    struct ValueObject;
//    template<typename T>
//    struct WeakRef;
//
//    template<typename T>
//    struct Ref {
//        template<typename ... Params>
//        Ref(Params... values) {
//            auto object = new_object<T>(values...);
//            ptr = object.get();
//            ptr->reference();
//            GC::GC::add_to_gc(std::move(object));
//        }
//
//        Ref(ValueObject<T> *ptr) : ptr(ptr) { ptr->reference(); }
//
//        Ref(const Ref<T> &ref) : ptr(ref.ptr) { ptr->reference(); }
//
//        ~Ref() { ptr->dereference(); }
//
//        operator WeakRef<T>() { return get_weak_reference(); }
//
//        WeakRef<T> get_weak_reference() const { return WeakRef<T>(ptr); }
//
//        T &operator*() {
//            return *ptr->ptr;
//        }
//
//        T *operator->() {
//            return ptr->ptr.get();
//        }
//
//        void mark(unsigned int new_mark) { ptr->mark(new_mark); }
//
//    private:
//        ValueObject<T> *ptr;
//
//        friend WeakRef<T>;
//    };

    struct Ptr;

    struct RefBase
    {
        RefBase() = default;
        RefBase(const RefBase& ref) : ptr(ref.ptr) { ptr->reference(); }
        RefBase(Ptr* pointer) : ptr(pointer) { ptr->reference(); }
        virtual ~RefBase() { ptr->dereference(); };

    protected:
        Ptr* ptr;
    };

    template<typename T, bool IsArray = std::is_array<T>::value>
    struct Ref {};

    template<typename T>
    struct Ref<T, false> : public RefBase
    {
        Ref(const Ref& ref) : RefBase(ref) {}
        Ref(Ptr *pointer) : RefBase(pointer) {}
        template<typename ... Param>
        Ref(Param... params) {
            auto pointer = Ptr::create<T>(params ...);
            pointer.reference();
            ptr = add(pointer);
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

    template<typename T>
    struct Ref<T, true> : public RefBase
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

        std::remove_extent_t<T>& operator[](std::ptrdiff_t n)
        {
            auto* typed_obj = static_cast<TypedObject<T>*>(ptr->get_object());
            return typed_obj->object[n];
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