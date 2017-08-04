#include "gc.hpp"

std::forward_list<std::unique_ptr<GC::Object>> GC::GC::used_list;
unsigned int GC::GC::current_mark { 1 };
