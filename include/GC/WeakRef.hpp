#pragma once

#include "GC/ValueObject.hpp"


namespace GC
{
    template <typename T> struct Ref;
    
    template<typename T>
    struct WeakRef
    {
        WeakRef() : ptr(nullptr) {};
        WeakRef(ValueObject<T>* ptr) : ptr(ptr)
        {
            if (ptr)
            {
                ptr->add_weak_reference();
            }
        }
        
        WeakRef(const WeakRef& ref) : ptr(ref.ptr)
        {
            if (ptr)
            {
                ptr->add_weak_reference();
            }
        }
        
        ~WeakRef()
        {
            if (ptr)
            {
                ptr->del_weak_reference();
            }
        }
        
        operator Ref<T>() { return get_reference(); }
        
        explicit operator bool() { return has_reference(); }
        
        Ref<T> get_reference() const
        {
            if (!ptr)
            {
                throw std::bad_weak_ptr();
            }
            return Ref<T>(ptr);
        }
        bool has_reference() const { return ptr != nullptr; }
        
    private:
        ValueObject<T>* ptr;
        
        friend Ref<T>;
    };
}
