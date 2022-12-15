//
// Created by yapeng-cheng on 22-12-15.
//

#include <iostream>
using std::cout, std::endl;

#include "mylist.h"
#include "mylist-iter.h"
#include "myfind.h"

int main(int argc, char * argv[])
{
    List<int> mylist;

    for (int i = 0; i < 5; i++)
    {
        mylist.insert_front(i);
        mylist.insert_end(i+2);
    }
    mylist.display();       // 4  3  2  1  0  2  3  4  5  6

    ListIter<ListItem<int>> begin(mylist.front());
    ListIter<ListItem<int>> end;    // default 0, nullptr
    ListIter<ListItem<int>> iter;   // default 0, nullptr

    iter = find(begin, end, 3);
    if (iter == end)
    {
        cout << "not fond !" << endl;
    }
    else
    {
        cout << "fond. value = " << iter->value() << endl;
    }

    iter = find(begin, end, 7);
    if (iter == end)
    {
        cout << "not fond !" << endl;
    }
    else
    {
        cout << "fond. value = " << iter->value() << endl;
    }

    return 0;
}