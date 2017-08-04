#pragma once

#include <algorithm>
#include <forward_list>
#include <iostream>
#include <memory>
#include <vector>


template<typename Object, typename T>
void mark(Object* object, unsigned int current_mark, T)
{
    object->set_mark(current_mark);
}

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
        
        std::function<void(unsigned int)> mark_func;
        
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
            mark_func = std::bind(mark<Object, T>, this, std::placeholders::_1, *ptr);
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
                    obj->mark_func(current_mark);
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
        Ref(const WeakRef<T>& wref)
        {
            ptr = wref.ptr;
            ptr->add_reference();
        }
        ~Ref() { ptr->del_reference(); }
        
        Ref<T>& operator=(const Ref<T>& ref)
        {
            ptr = ref.ptr;
            ptr->add_reference();
            
            return *this;
        }
        
        WeakRef<T> get_weak_reference() const { return WeakRef<T>(ptr); }
        
        T& operator*()
        {
            return *ptr->ptr;
        }
        
        T* operator->()
        {
            return ptr->ptr;
        }
        
        ValueObject<T>* get_pointer() { return ptr; }
        
    private:
        ValueObject<T>* ptr;
        
        friend WeakRef<T>;
    };
    
    template<typename T>
    struct WeakRef
    {
        WeakRef(ValueObject<T>* ptr) : ptr(ptr) { ptr->add_weak_reference(); }
        WeakRef(const WeakRef& ref) : ptr(ref.ptr) { ptr->add_weak_reference(); }
        WeakRef(const Ref<T>& ref)
        {
            ptr = ref.ptr;
            ptr->add_weak_reference();
        }
        ~WeakRef() { ptr->del_weak_reference(); }
        
        WeakRef<T>& operator=(const WeakRef<T>& ref)
        {
            ptr = ref.ptr;
            ptr->add_weak_reference();
            
            return *this;
        }
        
        Ref<T> get_reference() const { return Ref<T>(ptr); }
        
    private:
        ValueObject<T>* ptr;
        
        friend Ref<T>;
    };
    
    
    // Ref<->WeakRef conversion functions
    
    
    // Implementation functions
    auto weak_to_ref = [](auto w) { return w.get_reference(); };
    auto ref_to_weak = [](auto r) { return r.get_weak_reference(); };
    
    template<typename From, typename To, std::size_t ... Is>
    To to_ref_impl(const From& from, std::index_sequence<Is...>)
    {
        return To { weak_to_ref(std::get<Is>(from))... };
    }
    
    template<typename From, typename To, std::size_t ... Is>
    To to_weak_ref_impl(const From& from, std::index_sequence<Is...>)
    {
        return To { ref_to_weak(std::get<Is>(from))... };
    }
    
    
    // Single values
    template<typename T>
    Ref<T> to_ref(const WeakRef<T>& from)
    {
        return weak_to_ref(from);
    }
    
    template<typename T>
    WeakRef<T> to_weak_ref(const Ref<T>& from)
    {
        return ref_to_weak(from);
    }
    
    
    //Vectors
    template<typename T>
    std::vector<Ref<T>> to_ref(const std::vector<WeakRef<T>>& from)
    {
        std::vector<Ref<T>> to;
        
        std::transform(from.begin(),
                       from.end(),
                       std::back_inserter(to),
                       weak_to_ref);
        
        return to;
    }
    
    template<typename T>
    std::vector<WeakRef<T>> to_weak_ref(const std::vector<Ref<T>>& from)
    {
        std::vector<WeakRef<T>> to;
        
        std::transform(from.begin(),
                       from.end(),
                       std::back_inserter(to),
                       ref_to_weak);
        
        return to;
    }
    
    
    // Arrays
    template<typename T, std::size_t N>
    std::array<Ref<T>, N> to_ref(const std::array<WeakRef<T>, N>& from)
    {
        return to_ref_impl<std::array<WeakRef<T>, N>, std::array<Ref<T>, N>>(from, std::make_index_sequence<N>());
    }
    
    template<typename T, std::size_t N>
    std::array<WeakRef<T>, N> to_weak_ref(const std::array<Ref<T>, N>& from)
    {
        return to_weak_ref_impl<std::array<Ref<T>, N>, std::array<WeakRef<T>, N>>(from, std::make_index_sequence<N>());
    }
    
    
    // Tuples
    template<typename ... Ts>
    std::tuple<Ref<Ts>...> to_ref(const std::tuple<WeakRef<Ts>...>& from)
    {
        return to_ref_impl<std::tuple<WeakRef<Ts>...>,
                           std::tuple<Ref<Ts>...>>
            (from,
             std::make_index_sequence<std::tuple_size<std::tuple<WeakRef<Ts>...>>::value>());
    }
    
    template<typename ... Ts>
    std::tuple<WeakRef<Ts>...> to_weak_ref(const std::tuple<Ref<Ts>...>& from)
    {
        return to_weak_ref_impl<std::tuple<Ref<Ts>...>,
                                std::tuple<WeakRef<Ts>...>>
            (from,
             std::make_index_sequence<std::tuple_size<std::tuple<Ref<Ts>...>>::value>());
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
