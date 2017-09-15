#include "GC/Ref.hpp"
#include "GC/WeakRef.hpp"


namespace GC
{
    template<typename T>
    void traverse(Ref<T>& object, unsigned int marker)
    {
        object.ptr->mark(marker);
    }

    template<typename T>
    void traverse(WeakRef<T>& object, unsigned int marker)
    {
        auto ref = Ref<T>(object);
        ref.ptr->mark(marker);
    }
}