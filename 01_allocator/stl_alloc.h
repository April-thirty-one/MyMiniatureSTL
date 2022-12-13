//
// Created by yapeng-cheng on 22-12-12.
//

#ifndef MYMINIATURESTL_STL_ALLOC_H
#define MYMINIATURESTL_STL_ALLOC_H

#include <stdlib.h>
#include <stdio.h>


//// 以下是第一级配置器。 ----------------------------------------------------------------------------------------------------
//注意，无「template型别参数」。至于「非型别参数」inst，完全没派上用场。
template <int inst>
class __malloc_alloc_template
{
private:
    // 函数指针，处理内存不足情况
    // oom : 内存不足
    static void * oom_malloc(size_t);
    static void * oom_realloc(void *, size_t);
    static void (* __malloc_alloc_oom_headler)();

public:
    static void * allocate(size_t n);
    static void deallocate(void * p, size_t n);
    static void * reallocate(void * p, size_t old_size, size_t new_size);

    // 以下仿真 C++ 的 set_new_handler(), 也就是说，你可以通过它指定自己的 out-of-memory handler
    static void (* set_new_handler(void (*f)()))();
};



// malloc_alloc 内存不足处理
// 初始值设置为 0 ，有待客端设定
template <int inst>
void (* __malloc_alloc_template<inst>::__malloc_alloc_oom_headler)() = nullptr;   // 这是函数指针

template <int inst>
void * __malloc_alloc_template<inst>::oom_malloc(size_t n)
{
    void (* my_malloc_handler)();
    void * result;
    for ( ; ; )         // 不断尝试 释放 配置 再释放 再配置 ...
    {
        my_malloc_handler = __malloc_alloc_oom_headler;
        if (my_malloc_handler == nullptr)
        {
            fprintf(stderr, "out of memory\n");
            exit(1);
        }
        (* my_malloc_handler)();            // 调用处理例程，企图释放内存
        result = malloc(n);             // 再次尝试配置内存
        if (result)
            return result;
    }
}

template <int inst>
void * __malloc_alloc_template<inst>::oom_realloc(void * p, size_t n)
{
    void (* my_malloc_handler)();
    void * result;

    for ( ; ; )
    {
        my_malloc_handler = __malloc_alloc_oom_headler;
        if (my_malloc_handler == nullptr)
        {
            fprintf(stderr, "out of memory");
            exit(1);
        }
        (* my_malloc_handler)();
        result = realloc(p, n);             // realloc - 更改已经配置的内存空间，即更改由malloc()函数分配的内存空间的大小。
        if (result)
            return result;
    }
}


template <int inst>
void * __malloc_alloc_template<inst>::allocate(size_t n)
{
    // 第一级配置器直接使用malloc()
    void * result = malloc(n);

    // 若无法满足需求后，改用oom_malloc()
    if (result == nullptr)
    {
        result = oom_malloc(n);
    }

    return result;
}

template <int inst>
void __malloc_alloc_template<inst>::deallocate(void * p, size_t n)
{
    // 第一级配置器直接使用 free()
    free(p);
}

template <int inst>
void * __malloc_alloc_template<inst>::reallocate(void * p, size_t old_size, size_t new_size)
{
    void * result = realloc(p, new_size);
    if (result == nullptr)
    {
        result = oom_realloc(p, new_size);
    }

    return result;
}

// 以下仿真 C++ 的 set_new_handler(), 也就是说，你可以通过它指定自己的 out-of-memory handler
template <int inst>
void (* __malloc_alloc_template<inst>::set_new_handler(void (*f)()))()
{
    void (* old)() = __malloc_alloc_oom_headler;
    __malloc_alloc_oom_headler = f;
    return old;
}


//// 以下是第二级配置器。 ----------------------------------------------------------------------------------------------------

// 直接将 inst 参数设置为0
typedef __malloc_alloc_template<0> malloc_alloc;

enum
{
        __ALIGN = 8,                                // 小型区块的上调边界
        __MAX_BYTES = 128,                          // 小型区块的上限
        __NFREELLSTS = __MAX_BYTES / __ALIGN        // free-lists 的个数
};

// 无模板参数，且inst没用，第一个参数用于多线程情况，但是这里不讨论多线程
// 第二个参数完全没有派上用场
template <bool threads, int inst>
class __default_alloc_template
{
private:
    static size_t ROUND_UP(size_t bytes);           // ROUND_UP() 将bytes 上调到 8 的倍数
    union obj                           // free-lists 节点构造
    {
        union obj * free_list_link;
        char client_data[1];            // 用户可以看到这个
    };
    static obj * volatile free_list[__NFREELLSTS];      // 16 个 free_lists
    static size_t FREELIST_INDEX(size_t bytes);         // 根据区块大小，决定使用第n号 free_list, n 从 1 开始计数

    static void * refill(size_t n);                     // 当发现 free list 中没有可用区块时，就调用 refill()，准备为 free list重新填充空间，新的空间来自内存池，缺省取得20个新节点。返回一个大小为 n 的对象，并可能加入大小为n的其他区块到 free list。假设 n 已上调至 8 的倍数
    static char *chunk_alloc(size_t size, int & nobjs); // 若配置 nobjs 个区块有所不便，nobjs 可能会降低

    // 块分配状态
    static char * start_free;                           // 内存池的起始地址。只在 chunk_alloc() 中变换
    static char * end_free;                             // 内存池的结束地址。只在 chunk_alloc() 中变换
    static size_t heap_size;

public:
    static void * allocate(size_t n);                   // 此函数先判断区块大小，大于 128byte 就调用第一级分配器，反之就查找对应的 free list。若 free list 中有可用的区块，就直接拿来用，若没有可用的区块，就将区块大小上调至 8 的倍数，然后调用 refill(), 准备为 free list 重新填充空间。
    static void deallocate(void * p, size_t n);         // 此函数先判断区块大小，大于 128byte 就调用第一级分配器，反之就查找对应的 free list，将区块回收
    static void * reallocate(void * p, size_t old_size, size_t new_size);
};





template <bool threads, int inst>
size_t __default_alloc_template<threads, inst>::ROUND_UP(size_t bytes)
{
    return ((bytes + __ALIGN - 1) & ~(__ALIGN - 1));
}

template <bool threads, int inst>
size_t __default_alloc_template<threads, inst>::FREELIST_INDEX(size_t bytes)         // 根据区块大小，决定使用第n号 free_list, n 从 1 开始计数
{
    return ((bytes + __ALIGN - 1) / __ALIGN -1);
}

template <bool threads, int inst>
char * __default_alloc_template<threads, inst>::start_free = nullptr;

template <bool threads, int inst>
char * __default_alloc_template<threads, inst>::end_free = nullptr;

template <bool threads, int inst>
size_t __default_alloc_template<threads, inst>::heap_size = 0;

template <bool threads, int inst>
typename __default_alloc_template<threads, inst>::obj * volatile __default_alloc_template<threads, inst>::free_list[__NFREELLSTS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <bool threads, int inst>
void * __default_alloc_template<threads, inst>::allocate(size_t n)
{
    obj * volatile * my_free_list;
    obj * result;

    if (n > (size_t)__MAX_BYTES)        // 大于 128bytes 调用第一级配置器
    {
        return (malloc_alloc::allocate(n));
    }
    my_free_list = free_list + FREELIST_INDEX(n);   // 寻找16个free list 中最合适的一个
    result = * my_free_list;
    if (result == nullptr)              // 没有找到可用的 free list，准备重新填充 free list
    {
        void * r = refill(ROUND_UP(n));
        return r;
    }
    *my_free_list = result->free_list_link;     // 调整 free list
    return(result);
}

template <bool threads, int inst>
void __default_alloc_template<threads, inst>::deallocate(void *p, size_t n)
{
    obj * q = (obj *)p;
    obj * volatile * my_free_list;

    if (n > (size_t)__MAX_BYTES)    // 调用第一级配置器
    {
        malloc_alloc::deallocate(p, n);
        return;
    }
    my_free_list = free_list + FREELIST_INDEX(n);   // 查找对应的 free list

    q->free_list_link = *my_free_list;      // 调整 free list，回收区块
    *my_free_list = q;
}

template <bool threads, int inst>
void * __default_alloc_template<threads, inst>::refill(size_t n)
{
    int nobjs = 20;

    char * chunk = chunk_alloc(n, nobjs);            // 调用 chunk_alloc(), 尝试取得 nobjs 个区块作为 free list 的新节点
    obj * volatile my_free_list;
    obj * result;
    obj * current_obj;
    obj * next_obj;
    int i;

    if (nobjs == 1)         // 若只获得一个区块，这个区块分配给调用这使用，free list 无新节点
    {
        return (chunk);
    }

    // 否则准备调整 free list 纳入新节点
    my_free_list = free_list + FREELIST_INDEX(n);

    // 以下在 chunk 空间内建立 free list
    result = (obj *)chunk;      // 这一块准备返还给客端
    // 以下导引 free list 指向新配置的空间（取自内存池）
    *my_free_list = next_obj = (obj *)(chunk + n);
    // 以下将free list 的各个节点串联起来
    for (i = 1; ; i ++)         // 从 1 开始，因为第 0 个将返回客端
    {
        current_obj = next_obj;
        next_obj = (obj *)((char *)next_obj + n);
        if (nobjs - 1 == i)
        {
            current_obj->free_list_link = 0;
            break;
        }
        else
        {
            current_obj->free_list_link = next_obj;
        }
    }
    return result;
}






#endif //MYMINIATURESTL_STL_ALLOC_H
