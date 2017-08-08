#include <forward_list>
#include <memory>
#include <vector>

#include "GC/GC.hpp"
#include "GC/Object.hpp"


std::forward_list<std::unique_ptr<GC::Object>> GC::GC::used_list;
unsigned int GC::GC::current_mark { 1 };


void GC::GC::add_to_gc(std::unique_ptr<Object> object)
{
    collect();
    used_list.push_front(std::move(object));
}

std::vector<GC::Object*> GC::GC::get_roots()
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

void GC::GC::mark_objects(std::vector<Object*> roots)
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

void GC::GC::sweep()
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

void GC::GC::collect()
{
    auto roots = get_roots();
    mark_objects(roots);
    sweep();
    ++current_mark;
}