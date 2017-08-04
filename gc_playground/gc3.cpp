#include "gc3.hpp"

std::forward_list<std::unique_ptr<GC3::Object>> GC3::GC::used_list;
unsigned int GC3::GC::current_mark { 1 };


template<>
void mark(GC3::Object* object, unsigned int current_mark, Test3)
{
    auto* test_obj = dynamic_cast<GC3::ValueObject<Test3>*>(object);
    
    auto one_ref = test_obj->ptr->one.get_reference();
    auto two_ref = test_obj->ptr->two.get_reference();
    
    
}
