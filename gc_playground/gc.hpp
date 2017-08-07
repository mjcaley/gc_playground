#pragma once

#include <algorithm>
#include <forward_list>
#include <iostream>
#include <memory>
#include <vector>


template<typename O, typename T>
void traverse(O* object, unsigned int current_mark, T) {}

namespace GC
{
    struct Object
    {
        Object() : mark_count(0), ref_count(0), weak_ref_count(0) {}
        virtual ~Object() = default;
        
        unsigned int get_mark() const { return mark_count; }
        void set_mark(unsigned int mark_number) { mark_count = mark_number; }
        
        bool is_root() const { return ref_count > 0; }
        
        unsigned int get_ref_count() const { return ref_count; }
        void add_reference() { ++ref_count; }
        void del_reference() { --ref_count; }
        
        unsigned int get_weak_ref_count() const { return weak_ref_count; }
        void add_weak_reference() { ++weak_ref_count; }
        void del_weak_reference() { --weak_ref_count; }
        
        std::function<void(unsigned int)> traverse_func;
        void mark(unsigned int current_mark)
        {
            if (current_mark != mark_count)
            {
                mark_count = current_mark;
                traverse_func(current_mark);
            }
        }
        
    private:
        unsigned int mark_count;
        unsigned int ref_count;
        unsigned int weak_ref_count;
    };

    
    template<typename T> struct WeakRef;
    template<typename T> struct Ref;
    
    template<typename T>
    struct ValueObject : public Object
    {
        template<typename ... Params>
        ValueObject(Params... values) : ptr(std::make_unique<T>(values...))
        {
            traverse_func = std::bind(traverse<Object, T>, this, std::placeholders::_1, *ptr);
        }
        ~ValueObject() {}
        
        const std::unique_ptr<T> ptr;
    };
    
    template<typename T, typename ... Params>
    std::unique_ptr<ValueObject<T>> new_object(Params... values)
    {
        return std::make_unique<ValueObject<T>>(values...);
    }
    
    
    class GC
    {
    public:
        GC() = default;
        ~GC() = default;
        
        static void add_to_gc(std::unique_ptr<Object> object)
        {
            collect();
            used_list.push_front(std::move(object));
        }
        
        template<typename T, typename ... Params>
        static Ref<T> create(Params ... values)
        {
            auto object = new_object<T>(values...);
            auto reference = Ref<T>(object.get());
            add_to_gc(std::move(object));
            
            return reference;
        }
        
        static std::vector<Object*> get_roots()
        {
            std::vector<Object*> roots;
            
            for (auto& obj : used_list)
            {
                if (obj->is_root())
                {
                    roots.emplace_back(obj.get());
                }
            }
            
            return roots;
        }
        
        static void mark_objects(std::vector<Object*> roots)
        {
            for (auto obj : roots)
            {
                if (obj->get_mark() == current_mark)
                {
                    continue;
                }
                else
                {
                    obj->mark(current_mark);
                }
            }
        }
        
        static void sweep()
        {
            used_list.remove_if(
                                [](auto& obj) {
                                    bool result = obj->get_mark() != current_mark;
                                    if (result)
                                    {
                                        std::cout << "Sweeping object: " << &obj << std::endl;
                                    }
                                    return result;
                                }
                                );
        }
        
        static void collect()
        {
            auto roots = get_roots();
            mark_objects(roots);
            sweep();
            ++current_mark;
        }
        
    private:
        static std::forward_list<std::unique_ptr<Object>> used_list;
        static unsigned int current_mark;
    };

    template<typename T>
    struct Ref
    {
        template<typename ... Params>
        Ref(Params... values)
        {
            auto object = new_object<T>(values...);
            ptr = object.get();
            ptr->add_reference();
            GC::add_to_gc(std::move(object));
        }
        Ref(ValueObject<T>* ptr) : ptr(ptr) { ptr->add_reference(); }
        Ref(const Ref<T>& ref) : ptr(ref.ptr) { ptr->add_reference(); }
        ~Ref() { ptr->del_reference(); }
        
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
        
        ValueObject<T>* get_pointer() { return ptr; }
        
    private:
        ValueObject<T>* ptr;
        
        friend WeakRef<T>;
    };
    
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
    
    
    // Ref<->WeakRef conversion functions
    template<typename From, typename To, std::size_t ... Is>
    To convert_impl(From& from, std::index_sequence<Is...>)
    {
        return To { std::get<Is>(from)... };
    }
    
    template<typename T, std::size_t N>
    std::array<WeakRef<T>, N> convert(std::array<Ref<T>, N>& from)
    {
        return convert_impl<std::array<Ref<T>, N>, std::array<WeakRef<T>, N>>(from, std::make_index_sequence<N>());
    }
    
    template<typename T, std::size_t N>
    std::array<Ref<T>, N> convert(std::array<WeakRef<T>, N>& from)
    {
        return convert_impl<std::array<WeakRef<T>, N>, std::array<Ref<T>, N>>(from, std::make_index_sequence<N>());
    }
}




//std::ostream& operator<<(std::ostream& out, const GC::Object& obj)
//{
//    out << "GCObject2 { mark:" << obj.get_mark() <<
//    " ref_count:" << obj.get_ref_count() <<
//    " }";
//    return out;
//}
//
//template<typename T>
//std::ostream& operator<<(std::ostream& out, GC::Ref<T>& obj)
//{
//    out << "GCRef2 { " <<
//    "value: " << *obj <<
//    " }";
//    return out;
//}
