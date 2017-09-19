#pragma once

#include <functional>
#include <memory>
#include <type_traits>

#include "GC/construct_array.hpp"
#include "GC/Object.hpp"
#include "GC/Traverse.hpp"


namespace GC
{
    template<typename T, typename Enable> struct Ref;

    struct Ptr {
        Ptr(Ptr& pointer) : object(std::move(pointer.object)),
                            traverse_func(pointer.traverse_func),
                            mark_count(pointer.mark_count),
                            ref_count(pointer.ref_count),
                            weak_ref_count(pointer.weak_ref_count) {}
        Ptr(Ptr&& pointer) : object(std::move(pointer.object)),
                             traverse_func(pointer.traverse_func),
                             mark_count(pointer.mark_count),
                             ref_count(pointer.ref_count),
                             weak_ref_count(pointer.weak_ref_count) {}

        template<typename T, typename ... Param>
        static Ptr create(Param ... params)
        {
            auto typed_obj = std::make_unique<TypedObject<T>>(std::forward<Param>(params) ...);
            auto* raw_obj = typed_obj.get();
            auto traversal = [raw_obj](unsigned int marker) { traverse<T>(raw_obj->object, marker); };
            auto ptr = Ptr(std::move(typed_obj), traversal);
            return ptr;
        }

        template<typename T, typename ... Param>
        static Ptr create_traverse(std::function<void(unsigned int)> traverse_func, Param ... params)
        {
            auto typed_obj = std::make_unique<TypedObject<T>>(std::forward<Param>(params) ...);
            auto ptr = Ptr(std::move(typed_obj), traverse_func);
            return ptr;
        }

        Object* get_object() { return object.get(); }

        unsigned int get_mark() const { return mark_count; }

        void set_mark(unsigned int mark_number) { mark_count = mark_number; }

        bool is_root() const { return ref_count > 0; }

        unsigned int get_ref_count() const { return ref_count; }

        void reference() { ++ref_count; }

        void dereference()
        {
            --ref_count;
            free();
        }

        unsigned int get_weak_ref_count() const { return weak_ref_count; }

        void reference_weak() { ++weak_ref_count; }

        void dereference_weak()
        {
            --weak_ref_count;
            free();
        }

        void mark(unsigned int current_mark)
        {
            if (current_mark != mark_count) {
                mark_count = current_mark;
                if (traverse_func) {
                    traverse_func(current_mark);
                }
            }
        }

    private:
        std::unique_ptr<Object> object;

        unsigned int mark_count { 0 };
        unsigned int ref_count { 0 };
        unsigned int weak_ref_count { 0 };
        std::function<void(unsigned int)> traverse_func;

        Ptr(std::unique_ptr<Object> object, std::function<void(unsigned int)> traverse) :
                object(std::move(object)),
                traverse_func(std::move(traverse)) {}

        Ptr(std::unique_ptr<Object> object) : Ptr(std::move(object), nullptr) {}

        Ptr() : Ptr(nullptr, nullptr) {}

        void free()
        {
            if (ref_count == 0 && weak_ref_count == 0)
            {
                traverse_func = nullptr;
                object.reset();
            }
        }
    };
}
