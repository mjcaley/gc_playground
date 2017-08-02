#include "gc3.hpp"

std::forward_list<std::unique_ptr<GC3::Object>> GC3::GC::used_list;
unsigned int GC3::GC::current_mark { 1 };


template<>
void mark<int>(GC3::Object* object)
{
    std::cout << "int mark" << std::endl;
}

template<>
void mark<Test3>(GC3::Object* object)
{
    auto* test_obj = dynamic_cast<Test3*>(object);
    
    auto one_ref = test_obj->one.get_reference();
    auto two_ref = test_obj->two.get_reference();
    
//    mark<int>(one_ref.ptr);
//    mark<float>(two_ref.ptr);
}
