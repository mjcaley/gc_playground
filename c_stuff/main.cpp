#include <deque>
#include <functional>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <forward_list>
#include <tuple>


struct Allocation
{
    int current_mark { 0 };
    void* ptr;

    Allocation(Allocation&) = delete;
    Allocation(void* ptr) : ptr(ptr) {}
    Allocation(Allocation&& a) : ptr(std::move(a.ptr))
    {
        a.ptr = nullptr;
    }
    
    ~Allocation()
    {
        if (ptr)
        {
            std::free(ptr);
        }
    }
};


template<typename T>
struct Pointer
{
    using type = T;
    Allocation* alloc { nullptr };
    
    Pointer() {}
    Pointer(Allocation* a) : alloc(a) {}
    Pointer(Pointer& p) : alloc(p.alloc) {}
    
    type* cast()
    {
        return static_cast<type*>(alloc->ptr);
    }
    
    type* get()
    {
        if (alloc)
        {
            return cast();
        }
        else
        {
            return nullptr;
        }
    }
    
    type& operator*()
    {
        return *(get());
    }
    
    type* operator->()
    {
        return get();
    }
};



struct A
{
    int number;
};

struct B
{
    Pointer<A> a;
};




template<typename T> void mark(T&, int) { std::cout << "mark empty" << std::endl; }

template<typename T> void mark(Pointer<T>&, int);

void mark(A& a, int new_mark) {
    std::cout << "Mark A" << std::endl;
}

void mark(B& b, int new_mark)
{
    std::cout << "Mark B" << std::endl;
    mark(b.a, new_mark);
}

template<typename T>
void mark(Pointer<T>& p, int new_mark)
{
    std::cout << "Mark Pointer" << std::endl;
    if (p.alloc && p.alloc->current_mark != new_mark)
    {
        std::cout << "Marking allocation " << p.alloc << std::endl;
        p.alloc->current_mark = new_mark;
        mark(*p, new_mark);
    }
    else
    {
        std::cout << "Allocation already marked or nullptr" << std::endl;
    }
}

template<typename Iterator>
void mark(Iterator begin, Iterator end, int new_mark)
{
    std::cout << "Mark shadow stack" << std::endl;
    for (; begin != end; ++begin)
    {
        std::cout << "Calling mark" << std::endl;
        begin->mark(new_mark);
    }
}


struct StackItem
{
    StackItem(std::function<void(int)> mark_func) : mark(mark_func) {}
    
    std::function<void(int)> mark;
};

using ShadowStack = std::deque<StackItem>;

template<typename T>
void push(std::deque<StackItem>& shadow_stack, Pointer<T>& item)
{
    auto* alloc = item.alloc;
    
    shadow_stack.emplace_back(
        [alloc](int new_mark) { mark(*(static_cast<typename Pointer<T>::type*>(alloc->ptr)), new_mark); }
    );
}

void pop(std::deque<StackItem>& shadow_stack, int num)
{
    shadow_stack.erase(std::end(shadow_stack) - num, std::end(shadow_stack));
}



std::forward_list<Allocation> memory;


template<typename T>
Allocation allocate()
{
    return std::move(Allocation(std::calloc(1, sizeof(T))));
}

template<typename T>
Allocation* create(std::forward_list<Allocation>& memory)
{
    auto& alloc = memory.emplace_front(allocate<T>());
    return &alloc;
}

template<typename T>
void assign(Pointer<T>& p, Allocation* a)
{
    p.alloc = a;
}


Pointer<int> add(ShadowStack& shadow_stack, Pointer<int> left, Pointer<int> right)
{
    push(shadow_stack, left);
    push(shadow_stack, right);
    
    Pointer<int> result;
    push(shadow_stack, result);
    assign(result, create<int>(memory));
    *result = *left + *right;
    
    pop(shadow_stack, 3);
    
    return result;
}


void do_nothing(ShadowStack& shadow_stack, int num)
{
    std::cout << "In do_nothing" << std::endl;
    
    pop(shadow_stack, 0);
};

void no_param(ShadowStack& shadow_stack)
{
    std::cout << "In no_param" << std::endl;
    
    pop(shadow_stack, 0);
};

// Doesn't work for some reason
// template<typename Function, typename ... Arg>
// typename std::invoke_result_t<Function, Frame<Arg...>&, Arg...> call(const Function&& func, Arg&&... args)
// {
//     Frame<Arg&&...> f {};
//     if constexpr (std::is_void_v<typename std::invoke_result_t<Function, Frame<Arg...>, Arg...>>)
//     {
//         std::invoke(&func, std::forward<Frame<Arg&&...>>(f), std::forward<args>...);
//     }
//     else
//     {
//         return std::invoke(&func, std::forward<Frame<Arg&&...>>(f), std::forward<args>...);
//     }
// }



int entry(ShadowStack& shadow_stack)
{
    do_nothing(shadow_stack, 42);
    
    Pointer<int> one;
    assign(one, create<int>(memory));
    push(shadow_stack, one);
    *one = 1;
    Pointer<int> two;
    assign(two, create<int>(memory));
    push(shadow_stack, two);
    *two = 2;
    Pointer<int> result = add(shadow_stack, one, two);
    push(shadow_stack, result);
    
    Pointer<A> a;
    assign(a, create<A>(memory));
    a->number = 42;
    push(shadow_stack, a);
    
    Pointer<B> b;
    assign(b, create<B>(memory));
    b->a = a;
    push(shadow_stack, b);
    
    std::cout << "Add results: " << *result << std::endl;
    
    return 0;
}


int main()
{
    int* num = static_cast<int*>(std::malloc(sizeof(*num)));
    *num = 42;

    auto a1 = Allocation(static_cast<void*>(num));
    auto a2 = Allocation(std::move(a1));

    if (a1.ptr)
    {
        printf("Contents of a1: %i\n", *(static_cast<int*>(a1.ptr)) );
    }
    if (a2.ptr)
    {
        printf("Contents of a2: %i\n", *(static_cast<int*>(a2.ptr)) );
    }
    
    
    std::deque<StackItem> shadow_stack {};
    entry(shadow_stack);
    std::cout << "Shadow stack size " << shadow_stack.size() << std::endl;
    mark(shadow_stack.begin(), shadow_stack.end(), 1);
}
