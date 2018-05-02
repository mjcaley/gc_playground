#include <cstdlib>
#include "allocation.hpp"
    
void Allocation::release()
{
    std::free(pointer);
}
