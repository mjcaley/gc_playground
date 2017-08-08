#pragma once

#include <functional>

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
}
