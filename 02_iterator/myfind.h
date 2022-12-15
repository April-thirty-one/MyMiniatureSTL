//
// Created by yapeng-cheng on 22-12-15.
//

#ifndef MYMINIATURESTL_MYFIND_H
#define MYMINIATURESTL_MYFIND_H

template <typename InputIterator, typename T>
InputIterator find(InputIterator first, InputIterator last, const T & value)
{
    while (first != last && *first != value)
    {
        first ++;
    }
    return first;
}

#endif //MYMINIATURESTL_MYFIND_H
