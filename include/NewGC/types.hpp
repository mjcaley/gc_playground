#pragma once

#include <cstddef>
#include <functional>
#include <string>


using Bool = bool;
using Byte = std::byte;
using Int = std::int64_t;
using Float = double;



struct Collectable
{
    virtual ~Collectable() = default;

    virtual void mark_members(int mark) = 0;
};

template<typename T>
struct Function
{
    std::string name; // should be a reference to a global created by compiler

    std::function<T> function;
    // parameter types
    // function pointer
    // locals?
};


template<typename T>
struct Array : public Collectable
{
    Pointer<T> array;

    virtual void mark_members(int mark) override
    {
        // for ()
    };
};

struct Struct : public Collectable
{
    // Struct(Frame& f) { f.emplace_back(this); }
    virtual std::string name() = 0; // should be a reference to a global created by compiler

    // members
    virtual void mark_members(int mark) override = 0;
};
