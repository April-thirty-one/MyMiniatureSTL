//
// Created by yapeng-cheng on 22-12-15.
//

#ifndef MYMINIATURESTL_AUTO_PTR_H
#define MYMINIATURESTL_AUTO_PTR_H

template <class T>
class auto_ptr
{
public:
    explicit auto_ptr(T * p = nullptr) : pointee(p) {}
    template <typename U>
    auto_ptr(auto_ptr<U> & rhs) : pointee(rhs.release()) {}
    ~auto_ptr() {delete this->pointee;}

    template <typename U>
    auto_ptr<T> & operator=(auto_ptr<U> & rhs)
    {
        if (this == rhs)
        {
            return *this;
        }
        reset(rhs.release());
    }

    T & operator*() const {return *pointee;}
    T * operator->() const {return pointee;}
    T * get() const {return pointee;}

    // ...
private:
    T * pointee;
};

#endif //MYMINIATURESTL_AUTO_PTR_H
