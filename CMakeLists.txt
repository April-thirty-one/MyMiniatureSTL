cmake_minimum_required(VERSION 3.23)
project(MyMiniatureSTL)

set(CMAKE_CXX_STANDARD 11)

add_executable(MyMiniatureSTL main.cpp
        01_allocator/jjaloc.cpp 01_allocator/jjaloc.h 01_allocator/stl_construct_destroy.h 01_allocator/stl_alloc.h 01_allocator/stl_uninitialized.h 02_iterator/auto_ptr.h 02_iterator/mylist.h 02_iterator/mylist-iter.h 02_iterator/mylist-iter-test.cpp 02_iterator/myfind.h)
