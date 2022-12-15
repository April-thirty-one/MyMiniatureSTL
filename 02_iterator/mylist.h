//
// Created by yapeng-cheng on 22-12-15.
//

#ifndef MYMINIATURESTL_MYLIST_H
#define MYMINIATURESTL_MYLIST_H

#include <iostream>

template <class T>
class ListItem;

//// ================== 单向列表 声明 ==================
template <class T>
class List
{
public:
    List();
    void insert_front(const T & value);
    void insert_end(const T & value);
    void display(std::ostream & os = std::cout) const;
    ListItem<T> * front() const {return _front;}
    ListItem<T> * end() const {return _end;}
    // ...
private:
    ListItem<T> * _front;
    ListItem<T> * _end;
    long _size;
};

//// ================== 单向列表 实现 ==================
template <class T>
List<T>::List()
{
    _size = 0;
    _front = nullptr;
    _end = nullptr;
}

template <class T>
void List<T>::insert_front(const T & value)
{
    ListItem<T> * temp = new ListItem<T>(value);
    temp->set_next(_front);
    _front = temp;
    _size++;
}

template <class T>
void List<T>::insert_end(const T &value)
{
    ListItem<T> * temp = new ListItem<T>(value);
    if (_front == _end)
    {
        _front = temp;
        temp->set_next(_end);
        _end = nullptr;
    }
    ListItem<T> * last = _front;
    while (last->next() != _end)
    {
        last = last->next();
    }
    last->set_next(temp);
    temp->set_next(_end);
}

template <class T>
void List<T>::display(std::ostream &os) const
{
    ListItem<T> * temp = _front;
    while (temp != _end)
    {
        os << temp->value() << "  ";
        temp = temp->next();
    }
    os << std::endl;
}




//// ================== 单向列表节点 ==================
template <class T>
class ListItem
{
public:
    ListItem(const T & value) : _value(value) {_next = nullptr;}
    T value() const {return _value;}
    ListItem * next() const {return _next;}
    void set_next(ListItem * i) {_next = i;}
    T & operator*() const {return _value;}
    bool operator!=(const T & value) {return _value != value;}
    // ...
private:
    T _value;
    ListItem * _next;
};

#endif //MYMINIATURESTL_MYLIST_H
