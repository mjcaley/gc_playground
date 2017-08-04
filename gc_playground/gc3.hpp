#pragma once

#include <algorithm>
#include <forward_list>
#include <iostream>
#include <memory>
#include <vector>


namespace GC3
{
    struct Object
    {
        Object() : mark(0), ref_count(0), weak_ref_count(0) {}
        virtual ~Object() = default;
        
        unsigned int get_mark() const { return mark; }
        void set_mark(unsigned int mark_number) { mark = mark_number; }
        
        bool is_root() const { return ref_count > 0; }
        
        unsigned int get_ref_count() const { return ref_count; }
        void add_reference() { ++ref_count; }
        void del_reference() { --ref_count; }
        
        unsigned int get_weak_ref_count() const { return weak_ref_count; }
        void add_weak_reference() { ++weak_ref_count; }
        void del_weak_reference() { --weak_ref_count; }
        
        std::function<void(Object*)> traversal;
        
    private:
        unsigned int mark;
        unsigned int ref_count;
        unsigned int weak_ref_count;
    };
    
    
    template<typename T> struct WeakRef;
    template<typename T> struct Ref;
    
    template<typename T>
    struct ValueObject : public Object
    {
        ValueObject(T value)
        {
            ptr = std::make_unique<T>(value);
        }
        
        ~ValueObject() {}
        
    private:
        std::unique_ptr<T> ptr;
        
        friend WeakRef<T>;
        friend Ref<T>;
    };
    
    template<typename T>
    std::unique_ptr<ValueObject<T>> new_object(T value)
    {
        return std::make_unique<ValueObject<T>>(value);
    }
    
    
    class GC
    {
    public:
        GC() = default;
        ~GC() = default;
        
        //        template<typename T>
        //        static std::unique_ptr<ValueObject<T>> new_object(T value)
        //        {
        //            return std::make_unique<ValueObject<T>>(value);
        //        }
        
        static void add_to_gc(std::unique_ptr<Object> object)
        {
            used_list.push_front(std::move(object));
        }
        
        template<typename T>
        static Ref<T> create(T value)
        {
            //            collect();
            //            auto ptr = std::make_unique<ValueObject<T>>(value);
            //            Ref<T> reference(ptr.get());
            //            used_list.push_front(std::move(ptr));
            
            collect();
            auto object = new_object(value);
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
        
        static void mark(std::vector<Object*> roots)
        {
            for (auto obj : roots)
            {
                if (obj->get_mark() == current_mark)
                {
                    continue;
                }
                else
                {
                    obj->set_mark(current_mark);
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
            mark(roots);
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
        Ref() { ptr = nullptr; }
        Ref(T value)
        {
            auto object = new_object(value);
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
        
    private:
        ValueObject<T>* ptr;
        
        friend WeakRef<T>;
    };
    
    template<typename T>
    struct WeakRef
    {
        WeakRef() { ptr = nullptr; }
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
    
    auto weak_to_ref = [](auto w) { return w.get_reference(); };
    auto ref_to_weak = [](auto r) { return r.get_weak_reference(); };
    
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
    
    template<typename T, std::size_t N>
    std::array<Ref<T>, N> to_ref(const std::array<WeakRef<T>, N>& from)
    {
        std::array<Ref<T>, N> to;
        
        std::vector<Ref<T>> temp;
        std::transform(from.begin(),
                       from.end(),
                       std::back_inserter(temp),
                       weak_to_ref);
        std::copy(temp.begin(), temp.end(), to.begin());
        
        return to;
    }
    
    template<typename T, std::size_t N>
    std::array<WeakRef<T>, N> to_weak_ref(const std::array<Ref<T>, N>& from)
    {
        std::array<WeakRef<T>, N> to;
        
        std::vector<WeakRef<T>> temp;
        std::transform(from.begin(),
                       from.end(),
                       std::back_inserter(temp),
                       ref_to_weak);
        std::copy(temp.begin(), temp.end(), to.begin());
        
        return to;
    }
}


struct Test3
{
    GC3::WeakRef<int> one;
    GC3::WeakRef<float> two;
};

template<typename T>
void mark(GC3::Object* object) {}

//std::ostream& operator<<(std::ostream& out, const GC3::Object& obj)
//{
//    out << "GCObject2 { mark:" << obj.get_mark() <<
//    " ref_count:" << obj.get_ref_count() <<
//    " }";
//    return out;
//}
//
//template<typename T>
//std::ostream& operator<<(std::ostream& out, GC3::Ref<T>& obj)
//{
//    out << "GCRef2 { " <<
//    "value: " << *obj <<
//    " }";
//    return out;
//}
