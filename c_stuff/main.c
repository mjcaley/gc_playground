#include <stdlib.h>
#include <stdio.h>


typedef struct Allocation
{
    int mark;
    void* ptr;
} Allocation_t;

Allocation_t* allocation_init(size_t type, size_t num)
{
    Allocation_t* a = malloc(sizeof(*a));
    if (a)
    {
        a->mark = 0;
        a->ptr = calloc(num, type);
    }
    
    return a;
}

#define allocate(type, num) \
    (Allocation_t*)allocation_init(sizeof(type), num)


void allocation_destroy(Allocation_t* a)
{
    free(a->ptr);
    free(a);
}


// typedef struct Pointer
// {
//     Allocation_t* alloc;
// } Pointer_t;


// Pointer_t pointer_init(Allocation_t* alloc)
// {
//     Pointer_t p;
//     p.alloc = alloc;
    
//     return p;
// };

typedef struct Pointer_int
{
    Allocation_t* alloc;
} Pointer_int_t;

Pointer_int_t pointer_int_init(Allocation_t* alloc)
{
    Pointer_int_t p;
    p.alloc = alloc;
    
    return p;
}


typedef struct Pointer_double
{
    Allocation_t* alloc;
} Pointer_double_t;

Pointer_double_t pointer_double_init(Allocation_t* alloc)
{
    Pointer_double_t p;
    p.alloc = alloc;
    
    return p;
}


#define pointer_init(x, alloc) _Generic( x, int: pointer_int_init, \
                                                    double: pointer_double_init\
                                          )(alloc)


// void* get_int(Pointer_t* p)
// {
//     return p->alloc->ptr;
// }

// int* get(Pointer_int_t* p)
// {
//     return (int*)(p->alloc->ptr);
// }


// int* cast_to_int(Pointer_t* p)
// {
//     return (int*)(get(p));
// }




int main()
{
    // Allocation_t* a = allocation_init(1, int);
    Allocation_t* a = allocate(int, 1);
    
    *(int*)(a->ptr) = 42;
    
    printf("Value of the allocation is: %i\n", *(int*)(a->ptr));
    
    Pointer_int_t p = pointer_init(int, a);
    // printf("Value of pointer cast: %i\n", *(cast_to_int(&p)) );
    // printf("Value of pointer cast: %i\n", *(get(&p)) );
    
    allocation_destroy(a);
    
    return 0;
}
