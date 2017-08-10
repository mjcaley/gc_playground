#pragma once

#include <forward_list>
#include <iostream>
#include <memory>
#include <vector>

#include "GC/ValueObject.hpp"


namespace GC
{
    struct Object;
    template <typename T> struct Ref;
    
    class GC
    {
    public:
        GC() = default;
        ~GC() = default;
        
        static void add_to_gc(std::unique_ptr<Object> object);
        
        template<typename T, typename ... Params>
        static Ref<T> create(Params ... values)
        {
            auto object = new_object<T>(values...);
            auto reference = Ref<T>(object.get());
            add_to_gc(std::move(object));
            
            return reference;
        }
        
        static std::vector<Object*> get_roots();
        static void mark_objects(std::vector<Object*> roots);
        static void sweep();
        static void collect();
        
    private:
        static std::forward_list<std::unique_ptr<Object>> used_list;
        static unsigned int current_mark;

        struct AllocationCounter
        {
            std::uintmax_t existing { 0 };
            std::uintmax_t created { 0 };
            std::uintmax_t deleted { 0 };
            
            std::uintmax_t resolve()
            {
                existing += created;
                existing -= deleted;
                created = 0;
                deleted = 0;
                
                return existing;
            }
        };
        static AllocationCounter counter;
        
        static bool trigger_collection();
    };
}
