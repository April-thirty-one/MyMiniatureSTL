//
// Created by yapeng-cheng on 22-12-8.
//

#include "jjaloc.h"

#include <iostream>
#include <vector>

using std::cout, std::endl;

int main(int argc, char * argv[])
{
    int ia[5] = {0, 1, 2, 3, 4};
    std::vector<int, JJ::allocator<int> > iv(ia, ia + 5);
    for (int item : iv)
    {
        cout << item << "  ";
    }
    cout << endl;
    return 0;
}