#pragma once

#ifdef DEBUG

#include <iostream>

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::forward_list<T>& list)
{
    

    return out;
}

#endif
