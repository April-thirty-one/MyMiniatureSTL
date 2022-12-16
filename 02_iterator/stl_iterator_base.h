//
// Created by yapeng-cheng on 22-12-16.
//

#ifndef MYMINIATURESTL_STL_ITERATOR_BASE_H
#define MYMINIATURESTL_STL_ITERATOR_BASE_H

//// ============================== 简述 value_type 的萃取 ==============================
template <typename I>
struct value_traits          // traits : 特性
{
    typedef typename I::value_type value_type;
};

// 偏特化版本 -- 迭代器是一个原生指针
template <typename T>
struct value_traits<T *>
{
    typedef T value_type;
};

// 偏特化版本 -- 迭代器是一个 pointer-to-const，萃取出来的是 T，而不是 const T
template <typename T>
struct value_traits<const T *>
{
    typedef T value_type;
};


//// ============================== 正文 ==============================

//// 五种迭代器 声明
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <typename I>
struct iterator_traits
{
    typedef typename I::iterator_category   iterator_category;      // 五种 iterators
    typedef typename I::value_type          value_type;             // 迭代器所指对象的类别
    typedef typename I::difference_type     difference_type;        // 表示两个迭代器之间的距离，也可以表示容器的最大容量
    typedef typename I::pointer             pointer;                // 指针
    typedef typename I::reference           reference;              // 引用
};

// 偏特化版本 -- 迭代器是一个原生指针
template <typename T>
struct iterator_traits<T *>
{
    typedef random_access_iterator_tag      iterator_category;
    // ...
    typedef ptrdiff_t                       difference_type;
    typedef T *                             pointer;
    typedef T &                             reference;
};

// 偏特化版本 -- 迭代器是一个 pointer-to-const
template <typename T>
struct iterator_traits<const T *>
{
    typedef random_access_iterator_tag      iterator_category;
    // ...
    typedef ptrdiff_t                       difference_type;
    typedef const T *                       pointer;
    typedef const T &                       reference;
};



//// advance() 方法 -- 两个参数，迭代器 p，数值 n，将 p 累进 n 次。

template <typename InputIterator, typename Distance>
inline void advance(InputIterator & i, Distance n)
{
    __advance(i, n, iterator_traits<InputIterator>::iterator_category());       // iterator_traits<InputIterator>::iterator_category() -- 临时对象
}

// __advance() 方法的最后一个参数都只有类型说明，没有参数名称，因为它只是用来激活重载机制
template <typename InputIterator, typename Distance>
inline void __advance(InputIterator & i, Distance n, input_iterator_tag)        // 单向
{
    while (n --)
    {
        ++ i;
    }
}

template <typename ForwardIterator, typename Distance>
inline void __advance(ForwardIterator & i, Distance n, forward_iterator_tag)
{
    advance(i, n, input_iterator_tag());
}

template <typename BidirectionalIterator, typename Distance>
inline void __advance(BidirectionalIterator & i, Distance n, bidirectional_iterator_tag)        // 双向
{
    if (n >= 0)
    {
        while (n --)
        {
            ++ i;
        }
    }
    else
    {
        while (n ++)
        {
            -- i;
        }
    }
}

template <typename RandomAccessIterator, typename Distance>
inline void __advance(RandomAccessIterator & i, Distance n, random_access_iterator_tag)     // 双向，跳跃
{
    i += n;
}


//// distance() 方法 -- 计算两个迭代器之间的距离
template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last)
{
    typedef typename iterator_traits<InputIterator>::iterator_category category;
    return __distance(first, last, category());
}

template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type __distance(InputIterator first, InputIterator last, input_iterator_tag)
{
    // 因为迭代器是 inputIterator 类型，所以要 逐一累计距离
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last)
    {
        ++ first;
        ++ n;
    }
    return n;
}

template <typename RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type __distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
{
    // 因为迭代器是 random_access_iterator 类型，所以可以 直接计算
    return last - first;
}

#endif //MYMINIATURESTL_STL_ITERATOR_BASE_H
