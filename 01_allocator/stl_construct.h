//
// Created by yapeng-cheng on 22-12-9.
//

#ifndef MYMINIATURESTL_STL_CONSTRUCT_H
#define MYMINIATURESTL_STL_CONSTRUCT_H

//// 这里定义了全局函数 construct() 和 destroy()，负责对象的构造和析构

#include <new>

template <typename T1, typename T2>
inline void construct(T1 * p, const T2 & value)
{
    new (p) T1(value);          // placement new; 调用 T1::T1(value)
}

// 以下是第一个版本的 destroy(), 接受一个指针
template <typename T>
inline void destroy(T* pointer)
{
    pointer->~T();              // 调用 dtor->~T()
}

// 以下是第二个版本的 destroy(), 接受两个迭代器。
// 此函数设法找出元素的数值类型，进而利用 _type_traits<> 求取最合适的措施
template <typename ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
    __destroy(first, last, value_type(first));
}

// 判别元素的数值类型(value_type)是否有 普通析构函数(trivial destructor)
template <typename ForwardIterator, typename T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T *)
{
    typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
    __destroy_aux(first, last, trivial_destructor());
}

// 如果元素的数值类型(value_type)有 非普通析构函数(non-trivial destructor)
template <typename ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type)
{
    for (; first < last; first ++)
    {
        destroy(&*first);
    }
}

// 如果元素的数值类型(value_type)有 普通析构函数(trivial destructor)
template <typename ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type) {}

// 以下是 destroy() 的第二个版本针对迭代器为 char * 和 wchar * 的特化版本
inline void destroy(char *, char *) {}
inline void destroy(wchar_t *, wchar_t *) {}



#endif //MYMINIATURESTL_STL_CONSTRUCT_H
