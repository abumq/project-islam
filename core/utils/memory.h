#ifndef MEMORY_H
#define MEMORY_H

namespace memory {
template <typename T>
inline void deleteAll(T*& ptr) {
    if (ptr == nullptr) return;
    delete ptr;
    ptr = nullptr;
}

template <typename T>
inline void turnToNullPtr(T*& ptr) {
    ptr = nullptr;
}

template <typename Iterator, typename Container>
inline void clearPointerContainer(Iterator b, Iterator e, Container* container) {
    for (Iterator beg = b; beg != e; ++beg) {
        deleteAll(*beg);
    }
    container->clear();
}

#if !defined(_MSC_VER)
template <typename T, typename... T2>
inline void deleteAll(T*& pointer, T2*&... pointers) {
    memory::deleteAll(pointer);
    memory::deleteAll<T2...>(pointers...);
}

template <typename T, typename... T2>
inline void turnToNullPtr(T*& pointer, T2*&... pointers) {
    memory::turnToNullPtr(pointer);
    memory::turnToNullPtr<T2...>(pointers...);
}
#else
// Visual C++ (pre VS-2013) does not support varidiac templates hence we overload
template <typename T, typename T2>
inline void deleteAll(T*& pointer, T2*& pointer2) {
    memory::deleteAll(pointer);
    memory::deleteAll(pointer2);
}

template <typename T, typename T2, typename T3>
inline void deleteAll(T*& pointer, T2*& pointer2, T3*& pointer3) {
    memory::deleteAll(pointer);
    memory::deleteAll(pointer2);
    memory::deleteAll(pointer3);
}

template <typename T, typename T2, typename T3, typename T4>
inline void deleteAll(T*& pointer, T2*& pointer2, T3*& pointer3, 
        T4*& pointer4) {
    memory::deleteAll(pointer);
    memory::deleteAll(pointer2);
    memory::deleteAll(pointer3);
    memory::deleteAll(pointer4);
}

template <typename T, typename T2, typename T3, typename T4, typename T5>
inline void deleteAll(T*& pointer, T2*& pointer2, T3*& pointer3, 
        T4*& pointer4, T5*& pointer5) {
    memory::deleteAll(pointer);
    memory::deleteAll(pointer2);
    memory::deleteAll(pointer3);
    memory::deleteAll(pointer4);
    memory::deleteAll(pointer5);
}

template <typename T, typename T2>
inline void turnToNullPtr(T*& pointer, T2*& pointer2) {
    memory::turnToNullPtr(pointer);
    memory::turnToNullPtr(pointer2);
}

template <typename T, typename T2, typename T3>
inline void turnToNullPtr(T*& pointer, T2*& pointer2, T3*& pointer3) {
    memory::turnToNullPtr(pointer);
    memory::turnToNullPtr(pointer2);
    memory::turnToNullPtr(pointer3);
}

template <typename T, typename T2, typename T3, typename T4>
inline void turnToNullPtr(T*& pointer, T2*& pointer2, T3*& pointer3, 
        T4*& pointer4) {
    memory::turnToNullPtr(pointer);
    memory::turnToNullPtr(pointer2);
    memory::turnToNullPtr(pointer3);
    memory::turnToNullPtr(pointer4);
}

template <typename T, typename T2, typename T3, typename T4, typename T5>
inline void turnToNullPtr(T*& pointer, T2*& pointer2, T3*& pointer3, 
        T4*& pointer4, T5*& pointer5) {
    memory::turnToNullPtr(pointer);
    memory::turnToNullPtr(pointer2);
    memory::turnToNullPtr(pointer3);
    memory::turnToNullPtr(pointer4);
    memory::turnToNullPtr(pointer5);
}
#endif // defined(_MSC_VER)
} // namespace memory
#endif // MEMORY_H
