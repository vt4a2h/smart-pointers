#pragma once

#include <memory>

namespace smart_pointers {

    template<class Value, class Deleter = std::default_delete<Value>>
    class ScopedPointer
    {
    public:
        // You must call "delete" for pointer in deleter. If you don't do it, you'll have memory leaks.
        explicit ScopedPointer(Value *value, Deleter deleter = std::default_delete<Value>());
        ~ScopedPointer();

        void reset(Value *other = nullptr);

        explicit operator bool() const { return m_Value; }

        const Value *get() const { return m_Value; }
        Value *get() { return m_Value; }

        const Value& operator*() const { return *get(); }
        Value& operator*() { return *get(); }

        const Value *operator->() const { return get(); }
        Value *operator->() { return get(); }

        void swap(ScopedPointer &src);

    private:
        ScopedPointer(ScopedPointer &src) = delete;
        ScopedPointer(ScopedPointer &&src) = delete;
        ScopedPointer& operator= (ScopedPointer &rhs) = delete;
        ScopedPointer& operator= (ScopedPointer &&rhs) = delete;

        Value  *m_Value;
        Deleter m_Deleter;
    };

    template<class Value, class Deleter>
    ScopedPointer<Value, Deleter>::ScopedPointer(Value *value, Deleter deleter)
        : m_Value(value)
        , m_Deleter(deleter)
    {
    }

    template<class Value, class Deleter>
    ScopedPointer<Value, Deleter>::~ScopedPointer()
    {
        m_Deleter(m_Value);
    }

    template<class Value, class Deleter>
    void ScopedPointer<Value, Deleter>::reset(Value *other)
    {
        m_Deleter(m_Value);
        m_Value = other;
    }

    template<class Value, class Deleter>
    void ScopedPointer<Value, Deleter>::swap(ScopedPointer &src)
    {
        std::swap(m_Value, src.m_Value);
    }

}
