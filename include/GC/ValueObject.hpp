#pragma once

#include <functional>
#include <memory>

#include "GC/Object.hpp"


namespace GC
{
    template<typename T>
    struct ValueObject : public Object
    {
        template<typename ... Params>
        ValueObject(Params... values) : ptr(std::make_unique<T>(values...))
        {
            traverse_func = std::bind(traverse<T>, ptr.get(), std::placeholders::_1);
        }
        ~ValueObject() {}
        
        const std::unique_ptr<T> ptr;
    };
    
    template<typename T, typename ... Params>
    std::unique_ptr<ValueObject<T>> new_object(Params... values)
    {
        return std::make_unique<ValueObject<T>>(values...);
    }
}
