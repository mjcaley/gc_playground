#pragma once

#include <array>
#include <cstddef>
#include <functional>
#include <string>
#include <vector>


using Bool = bool;
using Byte = std::byte;
using Int = std::int64_t;
// using Float = double;





template<typename T>
struct Function
{
    using return_type = typename std::function<T>::result_type;
 
    std::string name;
    std::function<T> function;
};


template<typename T, std::size_t N>
struct Array
{
    std::array<T, N> array;
    
    int current_mark { -1 };
    void mark(int new_mark)
    {
        if (current_mark != new_mark)
        {
            for (auto& element : array)
            {
                element.mark(new_mark);
            }
        }
    };
};


struct Float
{
    std::string name() { return "Float"; }

    void mark(int new_mark) {};
    
    double value;
};


struct Example
{
    std::string name() { return "Exmaple"; }
    
    Int number { 0 };
    Pointer<Float> other_num;
    
    int current_mark { -1 };
    void mark(int new_mark)
    {
        if (current_mark != new_mark)
        {
            current_mark = new_mark;
            other_num.mark(new_mark);
        }
    }
};