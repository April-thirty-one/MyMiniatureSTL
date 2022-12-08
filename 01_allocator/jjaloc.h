//
// Created by yapeng-cheng on 22-12-8.
//

#ifndef MYMINIATURESTL_JJALOC_H
#define MYMINIATURESTL_JJALOC_H

// 在全局命名空间中有一个自带的、隐藏的operator new专门用来分配内存。
// 默认情况下编译器会将new这个关键字翻译成这个operator new和相应的构造函数。
// 但在有的情况下，用户自己会在类中重载operator new，这种情况下，编译器默认会使用类中重载的operator new
// （本质上因为编译器会从命名空间由内而外查找自己想要的函数，选用第一个）。
// 如果我们想要继续使用默认的operator new，就应该写成::new 字面意思就是调用最外层命名空间中的operator new
// 值得一提的是最外层的（也是默认的）operator new也是可以被重载的。通过这种方式我们可以改变所有new的部分行为。

#include <iostream>
#include <cstddef>  // for ptrdiff_t, size_t
#include <new>      // std::set_new_handler()
#include <climits>  // UINT_MAX

namespace JJ
{

// 配置空间，足以储存 n个 T对象。第二变量是个提示。可能会利用它来增进区域性（locality），或完全忽略之。
template <typename T>
inline T * _allocate(ptrdiff_t size, T *)
{
    std::set_new_handler(0);
//    T * temp = (T * )(::operator new((size_t)(size * sizeof(T))));
    T * temp = (T*) ::operator new((size_t)(size * sizeof(T)));                   //   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (temp == nullptr)
    {
        std::cout << "out of memory" << std::endl;
        exit(1);
    }
    return temp;
}

// 归还先前配置的空间
template <typename T>
inline void _deallocate(T * buffer)
{
    ::operator delete(buffer);
}

// 等同于 new(const void*) p) T(x)
template <typename T1, typename T2>
inline void _construct(T1 * p, const T2 & value)
{
    p = new T1(value);                                     // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

// 等同于 p->~T()
template <typename T>
inline void destroy(T * ptr)
{
    ptr->~T();
}

template <class T>
class allocator
{
public:
    typedef T          value_type;
    typedef T *         pointer;
    typedef const T *   const_pointer;
    typedef T &         reference;
    typedef const T &   const_reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;

    // 重新绑定类型为U的配置器

    pointer allocate(size_type n, const void * hint = nullptr)
    {
        return _allocate((difference_type)n, (pointer)0);
    }

    void deallocate(pointer p, size_type n)
    {
        _deallocate(p);
    }

    void construct(pointer p, const T & value)
    {
        _construct(p, value);
    }

    void destroy(pointer p)
    {
        _destroy(p);
    }

    // 传回某个对象的地址。a.address(x)等同于 &x
    pointer address(reference x)
    {
        return (pointer)&x;
    }

    // 传回某个 const对象的地址。a.address(x)等同于 &x
    const_pointer const_address(const_reference x)
    {
        return (const_pointer)&x;
    }

    // 传回可成功配置的最大量
    size_type max_size() const
    {
        return size_type(UINT_MAX / sizeof(T));
    }
};

}


#endif //MYMINIATURESTL_JJALOC_H






















