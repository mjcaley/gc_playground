#pragma once


namespace GC
{
    template <typename T> struct Ref;
    template<typename T> struct ValueObject;
    
    template<typename T>
    struct WeakRef
    {
        WeakRef() : ptr(nullptr) {};
        explicit WeakRef(ValueObject<T>* ptr) : ptr(ptr)
        {
            if (ptr)
            {
                ptr->reference_weak();
            }
        }
        
        WeakRef(const WeakRef& ref) : ptr(ref.ptr)
        {
            if (ptr)
            {
                ptr->reference_weak();
            }
        }
        
        ~WeakRef()
        {
            if (ptr)
            {
                ptr->dereference_weak();
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
