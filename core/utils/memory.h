#ifndef MEMORY_H
#define MEMORY_H

namespace memory {

template <typename T>
inline void deleteAll(T*& ptr) {
    if (ptr == nullptr) return;
    delete ptr;
    ptr = nullptr;
}

template <typename T, typename... T2>
inline void deleteAll(T*& pointer, T2*&... pointers) {
    memory::deleteAll(pointer);
    memory::deleteAll<T2...>(pointers...);
}

template <typename T>
inline void turnToNullPtr(T*& ptr) {
    ptr = nullptr;
}

template <typename T, typename... T2>
inline void turnToNullPtr(T*& pointer, T2*&... pointers) {
    memory::turnToNullPtr(pointer);
    memory::turnToNullPtr<T2...>(pointers...);
}

template <typename Iterator, typename Container>
inline void clearPointerContainer(Iterator b, Iterator e, Container* container) {
    for (Iterator beg = b; beg != e; ++beg) {
        deleteAll(*beg);
    }
    container->clear();
}

} // namespace memory
#endif // MEMORY_H
