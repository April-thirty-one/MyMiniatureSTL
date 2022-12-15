//
// Created by yapeng-cheng on 22-12-15.
//

#ifndef MYMINIATURESTL_MYLIST_ITER_H
#define MYMINIATURESTL_MYLIST_ITER_H

#include "mylist.h"

template <class Item>       // Item 可以是 单向列表的节点 或 双向列表的节点。
class ListIter              // 因为我们定义了独特的 opertor++，所以这里的 迭代器 只服务 List
{
public:
    ListIter(Item * p = nullptr) : ptr(p) {}
    ListIter(const ListIter<Item> & rhs) = default;
    ListIter<Item> & operator=(const ListIter<Item> & rhs) = default;

    Item & operator*() const {return *ptr;}
    Item * operator->() const {return ptr;}

    ListIter & operator++() {ptr = ptr->next(); return *this;}
    ListIter operator++(int) {ListIter temp = *this; ++*this; return temp;}           // postfix increment, 返回值（新对象）int为占位符，提示编译器这是后自增

    bool operator==(const ListIter & i) const {return this->ptr == i.ptr;}
    bool operator!=(const ListIter & i) const {return this->ptr != i.ptr;}
private:
    Item * ptr;             // 保持与容器之间的联系
};

#endif //MYMINIATURESTL_MYLIST_ITER_H
