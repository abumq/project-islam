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

template <typename T, typename T2, typename T3, typename T4, typename T5,
          typename T6>
inline void deleteAll(T*& pointer, T2*& pointer2, T3*& pointer3, 
        T4*& pointer4, T5*& pointer5, 
        T6*& pointer6) {
    memory::deleteAll(pointer);
    memory::deleteAll(pointer2);
    memory::deleteAll(pointer3);
    memory::deleteAll(pointer4);
    memory::deleteAll(pointer5);
    memory::deleteAll(pointer6);
}

template <typename T, typename T2, typename T3, typename T4, typename T5,
          typename T6, typename T7>
inline void deleteAll(T*& pointer, T2*& pointer2, T3*& pointer3, 
        T4*& pointer4, T5*& pointer5, 
        T6*& pointer6, T7*& pointer7) {
    memory::deleteAll(pointer);
    memory::deleteAll(pointer2);
    memory::deleteAll(pointer3);
    memory::deleteAll(pointer4);
    memory::deleteAll(pointer5);
    memory::deleteAll(pointer6);
    memory::deleteAll(pointer7);
}

template <typename T, typename T2, typename T3, typename T4, typename T5,
          typename T6, typename T7, typename T8>
inline void deleteAll(T*& pointer, T2*& pointer2, T3*& pointer3, 
        T4*& pointer4, T5*& pointer5, 
        T6*& pointer6, T7*& pointer7, T8*& pointer8) {
    memory::deleteAll(pointer);
    memory::deleteAll(pointer2);
    memory::deleteAll(pointer3);
    memory::deleteAll(pointer4);
    memory::deleteAll(pointer5);
    memory::deleteAll(pointer6);
    memory::deleteAll(pointer7);
    memory::deleteAll(pointer8);
}

template <typename T, typename T2, typename T3, typename T4, typename T5,
          typename T6, typename T7, typename T8, typename T9>
inline void deleteAll(T*& pointer, T2*& pointer2, T3*& pointer3, 
        T4*& pointer4, T5*& pointer5, 
        T6*& pointer6, T7*& pointer7, T8*& pointer8, 
        T9*& pointer9) {
    memory::deleteAll(pointer);
    memory::deleteAll(pointer2);
    memory::deleteAll(pointer3);
    memory::deleteAll(pointer4);
    memory::deleteAll(pointer5);
    memory::deleteAll(pointer6);
    memory::deleteAll(pointer7);
    memory::deleteAll(pointer8);
    memory::deleteAll(pointer9);
}

template <typename T, typename T2, typename T3, typename T4, typename T5,
          typename T6, typename T7, typename T8, typename T9, typename T10>
inline void deleteAll(T*& pointer, T2*& pointer2, T3*& pointer3, 
        T4*& pointer4, T5*& pointer5, 
        T6*& pointer6, T7*& pointer7, T8*& pointer8, 
        T9*& pointer9, T10*& pointer10) {
    memory::deleteAll(pointer);
    memory::deleteAll(pointer2);
    memory::deleteAll(pointer3);
    memory::deleteAll(pointer4);
    memory::deleteAll(pointer5);
    memory::deleteAll(pointer6);
    memory::deleteAll(pointer7);
    memory::deleteAll(pointer8);
    memory::deleteAll(pointer9);
    memory::deleteAll(pointer10);
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

template <typename T, typename T2, typename T3, typename T4, typename T5,
          typename T6>
inline void turnToNullPtr(T*& pointer, T2*& pointer2, T3*& pointer3, 
        T4*& pointer4, T5*& pointer5, 
        T6*& pointer6) {
    memory::turnToNullPtr(pointer);
    memory::turnToNullPtr(pointer2);
    memory::turnToNullPtr(pointer3);
    memory::turnToNullPtr(pointer4);
    memory::turnToNullPtr(pointer5);
    memory::turnToNullPtr(pointer6);
}

template <typename T, typename T2, typename T3, typename T4, typename T5,
          typename T6, typename T7>
inline void turnToNullPtr(T*& pointer, T2*& pointer2, T3*& pointer3, 
        T4*& pointer4, T5*& pointer5, 
        T6*& pointer6, T7*& pointer7) {
    memory::turnToNullPtr(pointer);
    memory::turnToNullPtr(pointer2);
    memory::turnToNullPtr(pointer3);
    memory::turnToNullPtr(pointer4);
    memory::turnToNullPtr(pointer5);
    memory::turnToNullPtr(pointer6);
    memory::turnToNullPtr(pointer7);
}

template <typename T, typename T2, typename T3, typename T4, typename T5,
          typename T6, typename T7, typename T8>
inline void turnToNullPtr(T*& pointer, T2*& pointer2, T3*& pointer3, 
        T4*& pointer4, T5*& pointer5, 
        T6*& pointer6, T7*& pointer7, T8*& pointer8) {
    memory::turnToNullPtr(pointer);
    memory::turnToNullPtr(pointer2);
    memory::turnToNullPtr(pointer3);
    memory::turnToNullPtr(pointer4);
    memory::turnToNullPtr(pointer5);
    memory::turnToNullPtr(pointer6);
    memory::turnToNullPtr(pointer7);
    memory::turnToNullPtr(pointer8);
}

template <typename T, typename T2, typename T3, typename T4, typename T5,
          typename T6, typename T7, typename T8, typename T9>
inline void turnToNullPtr(T*& pointer, T2*& pointer2, T3*& pointer3, 
        T4*& pointer4, T5*& pointer5, 
        T6*& pointer6, T7*& pointer7, T8*& pointer8, 
        T9*& pointer9) {
    memory::turnToNullPtr(pointer);
    memory::turnToNullPtr(pointer2);
    memory::turnToNullPtr(pointer3);
    memory::turnToNullPtr(pointer4);
    memory::turnToNullPtr(pointer5);
    memory::turnToNullPtr(pointer6);
    memory::turnToNullPtr(pointer7);
    memory::turnToNullPtr(pointer8);
    memory::turnToNullPtr(pointer9);
}

template <typename T, typename T2, typename T3, typename T4, typename T5,
          typename T6, typename T7, typename T8, typename T9, typename T10>
inline void turnToNullPtr(T*& pointer, T2*& pointer2, T3*& pointer3, 
        T4*& pointer4, T5*& pointer5, 
        T6*& pointer6, T7*& pointer7, T8*& pointer8, 
        T9*& pointer9, T10*& pointer10) {
    memory::turnToNullPtr(pointer);
    memory::turnToNullPtr(pointer2);
    memory::turnToNullPtr(pointer3);
    memory::turnToNullPtr(pointer4);
    memory::turnToNullPtr(pointer5);
    memory::turnToNullPtr(pointer6);
    memory::turnToNullPtr(pointer7);
    memory::turnToNullPtr(pointer8);
    memory::turnToNullPtr(pointer9);
    memory::turnToNullPtr(pointer10);
}
#endif // defined(_MSC_VER)
} // namespace memory
#endif // MEMORY_H
