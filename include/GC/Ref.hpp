#pragma once

#include <memory>

#include "GC/GC.hpp"
#include "GC/ValueObject.hpp"


namespace GC
{
    struct GC;
    template<typename T> struct WeakRef;
    
    template<typename T>
    struct Ref
    {
        template<typename ... Params>
        Ref(Params... values)
        {
            auto object = new_object<T>(values...);
            ptr = object.get();
            ptr->reference();
            GC::GC::add_to_gc(std::move(object));
        }
        Ref(ValueObject<T>* ptr) : ptr(ptr) { ptr->reference(); }
        Ref(const Ref<T>& ref) : ptr(ref.ptr) { ptr->reference(); }
        ~Ref() { ptr->dereference(); }
        
        operator WeakRef<T>() { return get_weak_reference(); }
        
        WeakRef<T> get_weak_reference() const { return WeakRef<T>(ptr); }
        
        T& operator*()
        {
            return *ptr->ptr;
        }
        
        T* operator->()
        {
            return ptr->ptr.get();
        }
        
        void mark(unsigned int new_mark) { ptr->mark(new_mark); }
        
    private:
        ValueObject<T>* ptr;
        
        friend WeakRef<T>;
    };
}
