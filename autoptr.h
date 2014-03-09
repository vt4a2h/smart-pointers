#pragma once

#include <memory> // for default_delete

namespace smart_pointers {

    template<class Value, class Deleter = std::default_delete<Value>>
    class AutoPointer
    {
    public:
        // You must call "delete" for pointer in deleter. If you don't do it, you'll have memory leaks.
        explicit AutoPointer(Value *value = nullptr, Deleter deleter = std::default_delete<Value>());
        AutoPointer(AutoPointer &src); // yes, without const
        AutoPointer(AutoPointer &&src) = delete;
        ~AutoPointer();

        AutoPointer& operator= (AutoPointer &rhs); // yes, without const
        AutoPointer& operator= (AutoPointer &&rhs) = delete;

        explicit operator bool() const { return m_Value; }

        Value *release();
        void reset(Value * other = nullptr);

        const Value *get() const { return m_Value; }
        Value *get() { return m_Value; }

        const Value& operator*() const { return *get(); }
        Value& operator*() { return *get(); }

        const Value *operator->() const { return get(); }
        Value *operator->() { return get(); }

    private:
        void swap(AutoPointer &src);

        Value *m_Value;
        Deleter m_Deleter;
    };

    template<class Value, class Deleter>
    AutoPointer<Value, Deleter>::AutoPointer(Value *value, Deleter deleter)
        : m_Value(value)
        , m_Deleter(deleter)
    {
    }

    template<class Value, class Deleter>
    AutoPointer<Value, Deleter>::AutoPointer(AutoPointer &src)
        : m_Value(nullptr)
        , m_Deleter(src.m_Deleter)
    {
        this->swap(src);
    }

    template<class Value, class Deleter>
    AutoPointer<Value, Deleter>::~AutoPointer()
    {
        this->m_Deleter(this->m_Value);
    }

    template<class Value, class Deleter>
    AutoPointer<Value, Deleter> &AutoPointer<Value, Deleter>::operator=(AutoPointer &rhs)
    {
        if (&rhs != this) {
            AutoPointer<Value, Deleter> tmp(rhs);
            this->swap(tmp);
        }

        return *this;
    }

    template<class Value, class Deleter>
    Value *AutoPointer<Value, Deleter>::release()
    {
        Value *tmp = m_Value;
        m_Value = nullptr;

        return tmp;
    }

    template<class Value, class Deleter>
    void AutoPointer<Value, Deleter>::reset(Value *other)
    {
        m_Deleter(m_Value);
        m_Value = other;
    }

    template<class Value, class Deleter>
    void AutoPointer<Value, Deleter>::swap(AutoPointer &src)
    {
        m_Deleter(m_Value);
        m_Value = nullptr;

        std::swap(m_Value, src.m_Value);
    }

}
