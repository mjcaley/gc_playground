#pragma once

#include <memory>
#include <type_traits>

#include "GC/Collector.hpp"
#include "GC/construct_array.hpp"


namespace GC {
    struct Ptr;
    template<typename T, typename Enable> struct Ref;

    template<typename T>
    struct WeakRef
    {
        WeakRef(const Ref<T>& ref) : ptr(ref.ptr) { ptr->reference_weak(); }
        WeakRef(Ptr *pointer) : ptr(pointer) { ptr->reference_weak(); }
        WeakRef(const WeakRef& ref) : ptr(ref.ptr) { ptr->reference_weak(); }
        virtual ~WeakRef() { ptr->dereference_weak(); };

    protected:
        Ptr* ptr;

    private:
        friend Ref<T>;
    };
}
