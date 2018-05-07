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



struct Collectable
{
    virtual ~Collectable() = default;

    Collectable* add_to_frame(Frame& frame)
    {
        frame.add_to_stack(this);
    }
    
    virtual std::vector<PointerBase*> get_pointers() = 0;
};

template<typename T>
struct Function// : public Collectable
{
    std::string name; // should be a reference to a global created by compiler

    std::function<T> function;
    // parameter types
    // function pointer
    // locals?
    
    // virtual void mark_members(int mark) {};
};


template<typename T, std::size_t N>
struct Array : public Collectable
{
    std::array<T, N> array;

    virtual std::vector<PointerBase*> get_pointers() override {};
};

template<typename T2, std::size_t N>
struct Array<Pointer<T2>, N> : public Collectable
{
    std::array<Pointer<T2>, N> array;
    
    virtual std::vector<PointerBase*> get_pointers() override
    {
        std::vector<PointerBase*> ptrs;
        for (auto& element : array)
        {
            ptrs.emplace_back(&element);
        }
        return ptrs;
    };
};


struct Struct : public Collectable
{
    // Struct(Frame& f) { f.emplace_back(this); }
    virtual std::string name() = 0; // should be a reference to a global created by compiler

    // members
    virtual std::vector<PointerBase*> get_pointers() = 0;
};


struct Float : public Struct
{
    virtual std::string name() override { return "Float"; }
    virtual std::vector<PointerBase*> get_pointers() override { return {}; };
    
    double value;
};


struct Example : public Struct
{
    virtual std::string name() override { return "Exmaple"; }
    
    Int number { 0 };
    Pointer<Float> other_num;
    
    virtual std::vector<PointerBase*> get_pointers() override
    {
        return { &other_num };
    }
};