#pragma once

#include <functional>
#include <memory>


namespace GC {
    struct Object {
        virtual ~Object() = default;
    };

    template<typename T>
    struct Ref;

    template<typename T>
    class TypedObject : public Object {
    public:
        template<typename ... Param>
        TypedObject(Param ... params) : object(T(params ...)) {}

    private:
        T object;

        friend Ref<T>;
    };

    template<typename T, std::size_t S>
    class TypedObject<std::array<T, S>> : public Object {
    public:
        template<typename ... Param>
        TypedObject(Param ... params) : object({params ...}) {}

    private:
        std::array<T, S> object;

        friend Ref<T[S]>;
    };

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
        static std::enable_if_t<!std::is_array<T>::value, Ptr>
        create(std::function<void(unsigned int)> traverse, Param ... params) {
            return Ptr(std::make_unique<TypedObject<T>>(params ...), traverse);
        };

        template<typename T, typename ... Param>
        static std::enable_if_t<!std::is_array<T>::value, Ptr>
        create(Param ... params) {
            return Ptr(std::make_unique<TypedObject<T>>(params ...));
        };

        template<typename T, typename ... Param>
        static std::enable_if_t<std::is_array<T>::value, Ptr>
        create(Param ... params) {
            return Ptr(std::make_unique<
                    TypedObject<std::array<std::remove_all_extents_t<T>, sizeof...(Param)>>>
                                   (params ...));
        }

        Object *get_object() { return object.get(); }

        unsigned int get_mark() const { return mark_count; }

        void set_mark(unsigned int mark_number) { mark_count = mark_number; }

        bool is_root() const { return ref_count > 0; }

        unsigned int get_ref_count() const { return ref_count; }

        void reference() { ++ref_count; }

        void dereference() { --ref_count; }

        unsigned int get_weak_ref_count() const { return weak_ref_count; }

        void reference_weak() { ++weak_ref_count; }

        void dereference_weak() { --weak_ref_count; }

        void mark(unsigned int current_mark) {
            if (current_mark != mark_count) {
                mark_count = current_mark;
                if (traverse_func) {
                    traverse_func(current_mark);
                }
            }
        }

    private:
        std::unique_ptr<Object> object;

        unsigned int mark_count;
        unsigned int ref_count;
        unsigned int weak_ref_count;
        std::function<void(unsigned int)> traverse_func;

        Ptr(std::unique_ptr<Object> object, std::function<void(unsigned int)> traverse) :
                mark_count(0),
                ref_count(0),
                weak_ref_count(0),
                object(std::move(object)),
                traverse_func(std::move(traverse)) {}

        Ptr(std::unique_ptr<Object> object) : Ptr(std::move(object), nullptr) {}

        Ptr() : Ptr(nullptr, nullptr) {}
    };
}
