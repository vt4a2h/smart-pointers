#pragma once

#include <memory>

namespace smart_pointers {

    template<class Value, class Deleter = std::default_delete<Value>>
    class SharedPointer
    {
    public:
        // You must call "delete" for pointer in deleter. If you don't do it, you'll have memory leaks.
        explicit SharedPointer(Value *value = nullptr, Deleter deleter = std::default_delete<Value>());
        SharedPointer(const SharedPointer &src);
        SharedPointer(SharedPointer &&src);
        ~SharedPointer();

        SharedPointer& operator= (const SharedPointer &rhs);
        SharedPointer& operator= (SharedPointer &&rhs);

        explicit operator bool() const { return m_Value; }

        Value *release();
        void reset(Value * other = nullptr);

        size_t useCount() const { return *m_LinkCounter; }
        bool   unique()   const { return *m_LinkCounter == 1; }

        const Value *get() const { return m_Value; }
        Value *get() { return m_Value; }

        const Value& operator*() const { return *get(); }
        Value& operator*() { return *get(); }

        const Value *operator->() const { return get(); }
        Value *operator->() { return get(); }

    private:
        void swap(SharedPointer &src);

        Value  *m_Value;
        size_t *m_LinkCounter;
        Deleter m_Deleter;
    };

    template<class Value, class Deleter>
    SharedPointer<Value, Deleter>::SharedPointer(Value *value, Deleter deleter)
        : m_Value(value)
        , m_LinkCounter(new size_t(0))
        , m_Deleter(deleter)
    {
        ++*m_LinkCounter;
    }

    template<class Value, class Deleter>
    SharedPointer<Value, Deleter>::SharedPointer(const SharedPointer &src)
        : m_Value(src.m_Value)
        , m_LinkCounter(src.m_LinkCounter)
        , m_Deleter(src.m_Deleter)
    {
        ++*m_LinkCounter;
    }

    template<class Value, class Deleter>
    SharedPointer<Value, Deleter>::SharedPointer(SharedPointer &&src)
        : m_Value(nullptr)
        , m_LinkCounter(nullptr)
        , m_Deleter(src.m_Deleter)
    {
        this->swap(src);
    }

    template<class Value, class Deleter>
    SharedPointer<Value, Deleter>::~SharedPointer()
    {
        if (m_LinkCounter && --*m_LinkCounter == 0) {
            m_Deleter(m_Value);
            delete m_LinkCounter;
        }
    }

    template<class Value, class Deleter>
    SharedPointer<Value, Deleter> &SharedPointer<Value, Deleter>::operator=(const SharedPointer &rhs)
    {
        if (&rhs != this) {
            SharedPointer<Value, Deleter> tmp(rhs);
            this->swap(tmp);
            ++*m_LinkCounter;
        }

        return *this;
    }

    template<class Value, class Deleter>
    SharedPointer<Value, Deleter> &SharedPointer<Value, Deleter>::operator=(SharedPointer &&rhs)
    {
        if (&rhs != this) {
            this->swap(rhs);
            ++*m_LinkCounter;
        }

        return *this;
    }

    template<class Value, class Deleter>
    Value *SharedPointer<Value, Deleter>::release()
    {
        Value *tmp = m_Value;
        m_Value = nullptr;

        return tmp;
    }

    template<class Value, class Deleter>
    void SharedPointer<Value, Deleter>::reset(Value *other)
    {
        m_Deleter(m_Value);
        m_Value = other;
    }

    template<class Value, class Deleter>
    void SharedPointer<Value, Deleter>::swap(SharedPointer &src)
    {
        m_Deleter(m_Value);
        m_Value = nullptr;

        std::swap(m_Value, src.m_Value);
        std::swap(m_LinkCounter, src.m_LinkCounter);
    }

}
