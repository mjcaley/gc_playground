#pragma once

#include <algorithm>
#include <forward_list>
#include <functional>
#include <memory>
#include <vector>


struct GCObject
{
    GCObject(int value, std::function<GCObject*(GCObject*)> traversal_function) :
    traversal_function(traversal_function),
    mark(0),
    ref_count(0)
    {
        ptr = std::make_unique<int>(value);
    };
    
    GCObject* traverse()
    {
        return traversal_function(this);
    }
    
    int& operator*() const
    {
        return *ptr.get();
    }
    
    int* operator->() const
    {
        return ptr.get();
    }
    
    void set_mark(unsigned int value) { mark = value; }
    int get_mark() const { return mark; }
    void add_reference() { ++ref_count; }
    void del_reference() { --ref_count; }
    bool is_root() const { return ref_count != 0; }
    
private:
    unsigned int mark;
    unsigned int ref_count;
    std::unique_ptr<int> ptr;
    std::function<GCObject*(GCObject*)> traversal_function;
};


struct GCRef
{
    GCRef(GCObject* ref) : ref(ref) { ref->add_reference(); }
    GCRef(const GCRef& gcref) : ref(gcref.ref) { ref->add_reference(); }
    ~GCRef() { ref->del_reference(); }
    
    int& operator*()
    {
        return ref->operator*();
    }
    
    int* operator->()
    {
        return ref->operator->();
    }
    
private:
    GCObject* ref;
};

GCObject* traverse_int(GCObject*)
{
    return nullptr;
}


class GC1
{
public:
    GC1() = default;
    
    std::vector<GCObject*> queue_roots(std::forward_list<GCObject>& used)
    {
        std::vector<GCObject*> roots;
        
        std::for_each(std::begin(used),
                      std::end(used),
                      [&roots](GCObject& obj)
                      {
                          if (obj.is_root())
                          {
                              roots.emplace_back(&obj);
                          }
                      });
        
        return roots;
    }
    
    void mark_children(GCObject* obj)
    {
        if (obj == nullptr)
        {
            return;
        }
        else if (obj->get_mark() == current_mark)
        {
            return;
        }
        else
        {
            obj->set_mark(current_mark);
            mark_children(obj->traverse());
        }
    }
    
    void sweep(std::forward_list<GCObject>& used)
    {
        used.remove_if(
                       [this](const GCObject& obj) {
                           bool result = obj.get_mark() != current_mark;
                           if (result)
                           {
                               std::cout << "Sweeping object: " << obj.operator*() << std::endl;
                           }
                           return result;
                       }
                       );
    }
    
    void collect()
    {
        auto roots = queue_roots(used);
        // Created lambda for function pointer, C++17 has std::invoke which looks better
        //        for (auto& root : roots)
        //        {
        //            mark_children(root);
        //        }
        auto mark_func = [this](GCObject* obj) { return this->mark_children(obj); };
        std::for_each(std::begin(roots), std::end(roots), mark_func);
        sweep(used);
        ++current_mark;
    }
    
    GCRef make_int(int value)
    {
        collect();
        used.emplace_front(GCObject(value, traverse_int));
        std::cout << "Allocated an integer: " << *used.front() << std::endl;
        return GCRef(&used.front());
    }
    
private:
    std::forward_list<GCObject> used;
    unsigned int current_mark { 1 };
};
