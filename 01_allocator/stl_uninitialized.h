//
// Created by yapeng-cheng on 22-12-14.
//

#ifndef MYMINIATURESTL_STL_UNINITIALIZED_H
#define MYMINIATURESTL_STL_UNINITIALIZED_H

#include <cstring>
#include "stl_construct_destroy.h"


//// =================================== 以下是 unitialized_fill_n() 的源码 ===================================
//迭代器 first -- 指向欲初始化空间的起始地址
//      n     -- 表示欲初始化空间的大小
//      x     -- 表示初值
template <typename ForwardIterator, typename Size, typename T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T & x)
{
    return __uninitializer_fill_n(first, n, x, value_type(first));      // 利用value_type 萃取出迭代器 first 的 value type。 value_type 的用法见 ../02_Iterator
}

/*
 * 首先萃取出迭代器 first 的 value type，然后判断是否为 POD 类型
 * POD 意指 Plain Old Data，也就是标量型别 或 传统的 C struct 类型。
 */
template <typename ForwardIterator, typename Size, typename T, typename T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T & x, T1 *)
{
    typedef typename  __type_traits<T1>::is_POD_type is_POD;        // __type_traits 的用法见 ../02_Iterator
    return __uninitialized_fill_n_aux(first, n, x, is_POD());
}

/*
 * POD 类型必然拥有 trivial ctor / dtor / copy / assignment 函数
 * 因此，我们可以对 POD 型别采取最有效率的初值填写方法，而对 non-POD 型别采取最保险安全的做法：
 *
 * 如果 copy construction 等同于 assignment，而且 destructor 是 trivial，以下就生效
 * 若果是 POD 型别，执行流程就会转进到以下函数，这是籍由 function template 的参数推导机制而得
 */
template <typename ForwardIterator, typename Size, typename T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T & x, __true_type)
{
    return fill_n(first, n, x);             // 交给高阶函数来执行 见 6.4.2 节
}

/*
 * 若果不是 POD 型别，执行流程就会转进到以下函数，这是籍由 function template 的参数推导机制而得
 */
template <typename ForwardIterator, typename Size, typename T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T & x, __false_type)
{
    ForwardIterator cur = first;
    for ( ; n > 0; --n, ++ cur)
    {
        construct(&*cur, x);
    }
    return cur;
}


//// =================================== 以下是 unitialized_copy() 的源码 ===================================
/*
 * 迭代器 first 指向输入端的起始位置
 * 迭代器 last 指向输入端的结束位置(前闭后开区间)
 * 迭代器 result 指向输入端(欲初始化空间)的起始位置
 */
template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
{
    return __uninitialized_copy(first, last, value_type(result));       // 利用value_type 萃取出迭代器 first 的 value type。
}

/*
 * 然后判断是否是 POD 型别
 */
template <typename InputIterator, typename ForwardIterator, typename T>
inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T *)
{
    typedef typename __type_traits<T>::is_POD_type is_POD;
    return __uninitialized_copy_aux(first, last, result, is_POD());     // 尝试利用 is_POD() 所获得的结果，让编译器作推导
}

/*
 * POD 类型必然拥有 trivial ctor / dtor / copy / assignment 函数
 * 因此，我们可以对 POD 型别采取最有效率的初值填写方法，而对 non-POD 型别采取最保险安全的做法：
 *
 * 如果 copy construction 等同于 assignment，而且 destructor 是 trivial，以下就生效
 * 若果是 POD 型别，执行流程就会转进到以下函数，这是籍由 function template 的参数推导机制而得
 */
template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type)
{
    return copy(first, last, result);           // 调用 STL 的 copy 算法
}

/*
 * 若果不是 POD 型别，执行流程就会转进到以下函数，这是籍由 function template 的参数推导机制而得
 */
template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type)
{
    ForwardIterator cur = result;
    for ( ; first != last; ++first, ++cur)
    {
        construct(&*cur, *first);
    }
    return cur;
}

/*
 * 针对 char * 和 wchar_t * 两种型别，可以采用最有效率的方法 memmove(直接移动内存内容)来执行复制行为。
 * 因此可以专门设置一套 特化版本
 */
// 以下是针对 const char * 的特化版本
inline char * uninitialized_copy(const char * first, const char * last, char * result)
{
    memmove(result, first, last - first);
    return result + (last - first);
}

// 以下是针对 const wchar_t * 的特化版本
inline wchar_t * uninitialized_copy(const wchar_t * first, const wchar_t * last, wchar_t * result)
{
    memmove(result, first, sizeof(wchar_t) * (last - first));
    return result + (last - first);
}


//// =================================== 以下是 unitialized_fill() 的源码 ===================================



#endif //MYMINIATURESTL_STL_UNINITIALIZED_H
