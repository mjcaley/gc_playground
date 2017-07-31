#include <iostream>
#include <forward_list>
#include <functional>
#include <memory>


struct GCObject
{
    GCObject(int value, std::function<void(GCObject*)> traversal_function) :
        traverse(traversal_function),
        mark(0),
        ref_count(0)
    {
        ptr = std::make_unique<int>(value);
    };
    
    const std::function<void(GCObject*)> traverse;
    
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

void traverse_int(GCObject*) {}


class GC
{
public:
    GC() = default;
    
    void mark(std::forward_list<GCObject>& used)
    {
        for (auto& obj : used)
        {
            if (obj.is_root())
            {
                obj.set_mark(current_mark);
            }
        }
    }
    
    void sweep(std::forward_list<GCObject>& used)
    {
        used.remove_if(
            [this](const GCObject& obj) {
                std::cout << "Sweeping object: " << obj.operator*() << std::endl;
                return obj.get_mark() != current_mark;
            }
        );
    }
    
    void collect()
    {
        mark(used);
        sweep(used);
        ++current_mark;
    }
    
    GCRef make_int(int value)
    {
        used.emplace_front(GCObject(value, traverse_int));
        return GCRef(&used.front());
    }
    
private:
    std::forward_list<GCObject> used;
    unsigned int current_mark { 1 };
};


void sub_func(GC& gc)
{
    // Allocate object and let it die
    
    auto i42 = gc.make_int(42);
    std::cout << "Allocated an integer: " << *i42 << std::endl;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    auto gc = GC();
    sub_func(gc);
    gc.collect();
    
    return 0;
}
