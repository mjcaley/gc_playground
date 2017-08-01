#pragma once

#include <forward_list>
#include <memory>


struct GCObject2
{
    GCObject2() : mark(0), ref_count(0) {}
    virtual ~GCObject2() { /*std::cout << "GCObject2 destructor called" << std::endl;*/ }
    
    unsigned int get_mark() const { return mark; }
    void set_mark(unsigned int mark_number) { mark = mark_number; }
    
    bool is_root() const { return ref_count > 0; }
    
    unsigned int get_ref_count() const { return ref_count; }
    void add_reference() { std::cout << "Ref++\n"; ++ref_count; }
    void del_reference() { std::cout << "Ref--\n"; --ref_count; }
    
private:
    unsigned int mark;
    unsigned int ref_count;
};

std::ostream& operator<<(std::ostream& out, const GCObject2& obj)
{
    out << "GCObject2 { mark:" << obj.get_mark() <<
        " ref_count:" << obj.get_ref_count() <<
        " }";
    return out;
}


template<typename T>
struct GCRef2;

template<typename T>
struct ValueObject : public GCObject2
{
    ValueObject(T value)
    {
        ptr = std::make_unique<T>(value);
    }
    
    ~ValueObject() { std::cout << "ValueObject destructor called" << std::endl; }
    
private:
    std::unique_ptr<T> ptr;
    
    friend GCRef2<T>;
};

template<typename T>
struct GCRef2
{
    GCRef2(ValueObject<T>* ptr) : ptr(ptr) { ptr->add_reference(); }
    GCRef2(const GCRef2& gcref) : ptr(gcref.ptr) { ptr->add_reference(); }
    ~GCRef2() { ptr->del_reference(); }
    
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
    
//    friend std::ostream& operator<<(std::ostream& out, const GCRef2<T>& obj);
};

template<typename T>
std::ostream& operator<<(std::ostream& out, GCRef2<T>& obj)
{
    out << "GCRef2 { " <<
        "value: " << *obj <<
        " }";
    return out;
}


//template<typename T, typename... Args>
//void traverse(GCObject2* obj) {}



class GC2
{
public:
    GC2() = default;
    
    template<typename T>
    GCRef2<T> create(T value)
    {
        collect();
        auto ptr = std::make_unique<ValueObject<T>>(value);
        GCRef2<T> reference(ptr.get());
        used_list.push_front(std::move(ptr));
        
        return reference;
    }
    
    std::vector<GCObject2*> get_roots()
    {
        std::vector<GCObject2*> roots;
        
        for (auto& obj : used_list)
        {
            if (obj->is_root())
            {
                roots.emplace_back(obj.get());
            }
        }
        
        return roots;
    }
    
    void mark(std::vector<GCObject2*> roots)
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
    
    void sweep()
    {
        used_list.remove_if(
                       [this](auto& obj) {
                           bool result = obj->get_mark() != current_mark;
                           if (result)
                           {
                               std::cout << "Sweeping object: " << &obj << std::endl;
                           }
                           return result;
                       }
                       );
    }
    
    void collect()
    {
        auto roots = get_roots();
        mark(roots);
        sweep();
        ++current_mark;
    }
    
private:
    std::forward_list<std::unique_ptr<GCObject2>> used_list;
    unsigned int current_mark { 1 };
};
