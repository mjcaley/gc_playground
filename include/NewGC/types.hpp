#pragma once

#include <array>
#include <cstddef>
#include <functional>
#include <string>
#include <vector>

#include "collectable.hpp"


using Bool = bool;
using Byte = std::byte;
using Int = std::int64_t;
// using Float = double;





template<typename T>
struct Function// : public Collectable
{
    std::string name; // should be a reference to a global created by compiler

    std::function<T> function;
    // parameter types
    // function pointer
    // locals?
    using return_type = typename std::function<T>::result_type;
    
    // virtual void mark_members(int mark) {};
};


template<typename T, std::size_t N>
struct Array : public Collectable
{
    std::array<T, N> array;
    
    virtual void mark(int new_mark) override
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

// template<typename T2, std::size_t N>
// struct Array<Pointer<T2>, N> : public Collectable
// {
//     std::array<Pointer<T2>, N> array;
    
//     virtual void mark(int new_mark) override
//     {
//         if (current_mark != new_mark)
//         {
//             for (auto& element : array)
//             {
//                 element.mark(new_mark));
//             }
//         }
//     };
// };


struct Struct : public Collectable
{
    // Struct(Frame& f) { f.emplace_back(this); }
    virtual std::string name() = 0; // should be a reference to a global created by compiler

    // members
    virtual void mark(int new_mark) = 0;
};


struct Float //: public Collectable
{
    /*virtual*/ std::string name() /*override*/ { return "Float"; }
    /*virtual*/ void mark(int new_mark) /*override*/ {
        std::cout << "Float mark!" << std::endl;
    };
    
    double value;
};


struct Example : public Struct
{
    virtual std::string name() override { return "Exmaple"; }
    
    Int number { 0 };
    Pointer<Float> other_num;
    
    virtual void mark(int new_mark) override
    {
        if (current_mark != new_mark)
        {
            current_mark = new_mark;

            // number.mark(new_mark);
            other_num.mark(new_mark);
        }
    }
};